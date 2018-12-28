#include <cglm/cglm.h>
#include <stb/stb_image.h>

#include "graphics.h"
#include "game.h"
#include "util.h"
#include "resources/shaders/basic.vert.h"
#include "resources/shaders/basic.frag.h"

void render(GLFWwindow const * const window, struct world* world) {
	p_log("Render called\n");
}

struct draw_info _draw_info_create() {
	GLuint vao;
	glGenVertexArrays(1, &vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);

	GLuint ebo;
	glGenBuffers(1, &ebo);

	struct draw_info ret = {
		.vao = vao,
		.vbo = vbo,
		.ebo = ebo,
		.n_indices = -1,
	};

	return ret;
}

void draw_info_activate(struct draw_info const* info) {
	glBindVertexArray(info->vao);
}

void draw_info_destroy(struct draw_info* info) {
	glDeleteVertexArrays(1, &info->vao);
	glDeleteBuffers(1, &info->vbo);
	glDeleteBuffers(1, &info->ebo);
}

// TODO: Parameterize this over UV
struct draw_info sprite_create() {
	struct draw_info sprite = _draw_info_create();

	glBindVertexArray(sprite.vao);
	glBindBuffer(GL_ARRAY_BUFFER, sprite.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite.ebo);

	GLfloat vertex_data[20] = {
		//   x    y    z    u    v
		   0.0,  1.0, 0.0, 0.0, 0.0,
		   1.0,  1.0, 0.0, 1.0, 0.0,
		   1.0,  0.0, 0.0, 1.0, 1.0,
		   0.0,  0.0, 0.0, 0.0, 1.0,
	};
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(GLfloat), vertex_data, GL_STATIC_DRAW);
	glVertexAttribPointer(POS_ATTRIB, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glVertexAttribPointer(UV_ATTRIB, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(POS_ATTRIB);
	glEnableVertexAttribArray(UV_ATTRIB);

	GLuint indices[6] = { 0, 1, 3, 1, 2, 3 };
	sprite.n_indices = 6;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	return sprite;
}

void draw_info_draw(struct draw_info const* info) {
	glDrawElements(GL_TRIANGLES, info->n_indices, GL_UNSIGNED_INT, 0);
}

struct shader_info _shader_info_create() {
	GLuint vert_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag_id = glCreateShader(GL_FRAGMENT_SHADER);

	struct shader_info ret = {
		.vert = vert_id,
		.frag = frag_id,
		.program = 0,
	};

	return ret;
}

void shader_info_destroy(struct shader_info* info) {
	glDeleteShader(info->vert);
	glDeleteShader(info->frag);
	glDeleteProgram(info->program);
}

void shader_info_activate(struct shader_info const* info) {
	glUseProgram(info->program);
}
	
struct shader_info basic_shader_create(char* err_buf, size_t err_buf_sz) {
	struct shader_info shader = _shader_info_create();

	GLint len = basic_vert_len;
	GLchar const* src = (GLchar const*)basic_vert;
	glShaderSource(shader.vert, 1, &src, &len);
	glCompileShader(shader.vert);
	GLint success = 0;
	glGetShaderiv(shader.vert, GL_COMPILE_STATUS, &success);
	if (!success) {
		p_debug("Vert broke :(\n");
		glGetShaderInfoLog(shader.vert, err_buf_sz, NULL, err_buf);
		return shader;
	}

	len = basic_frag_len;
	src = (GLchar const*)basic_frag;
	glShaderSource(shader.frag, 1, &src, &len);
	glCompileShader(shader.frag);
	glGetShaderiv(shader.frag, GL_COMPILE_STATUS, &success);
	if (!success) {
		p_debug("Frag broke :(\n");
		glGetShaderInfoLog(shader.frag, err_buf_sz, NULL, err_buf);
		return shader;
	}

	shader.program = glCreateProgram();
	glAttachShader(shader.program, shader.vert);
	glAttachShader(shader.program, shader.frag);
	glLinkProgram(shader.program);

	return shader;
}

int uniform_mat4f_enable(GLuint program, struct uniform_mat4f const uniform) {
	GLint location = glGetUniformLocation(program, uniform.name);
	if (location < 0) {
		return -1;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, uniform.data);
	return 0;
}

struct texture_info _texture_info_create() {
	struct texture_info ret = {0};
	glGenTextures(1, &ret.texture);

	return ret;
}

void texture_info_destroy(struct texture_info* info) {
	glDeleteTextures(1, &info->texture);
}

struct texture_info texture_info_stbi_load_memory(unsigned char const* data, size_t len) {
	struct texture_info info = _texture_info_create();
	int x = 0;
	int y = 0;
	int comp = 0;
	unsigned char* stbi_data = stbi_load_from_memory(data, len, &x, &y, &comp, 4);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, info.texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, stbi_data);
	stbi_image_free(stbi_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return info;
}

int uniform_texture_enable(GLuint program, struct uniform_texture uniform, GLuint slot) {
	GLint location = glGetUniformLocation(program, uniform.name);
	if (location < 0) {
		return -1;
	}
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, uniform.info->texture);
	glUniform1i(location, GL_TEXTURE0 + slot);
	return 0;
}
