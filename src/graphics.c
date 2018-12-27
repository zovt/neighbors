#include "graphics.h"
#include "util.h"
#include "resources/shaders/basic.vert.h"
#include "resources/shaders/basic.frag.h"

void render(GLFWwindow const * const window, struct draw_info const* sprite, struct shader_info* basic) {
	p_log("Render called\n");
	shader_info_activate(basic);
	draw_info_activate(sprite);
	draw_info_draw(sprite);
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
		  -1.0,  1.0, 0.0, 0.0, 0.0,
		   0.0,  1.0, 0.0, 0.0, 0.0,
		   0.0,  0.0, 0.0, 0.0, 0.0,
		  -1.0,  0.0, 0.0, 0.0, 0.0,
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
	GLchar const* src = basic_vert;
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
	src = basic_frag;
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

int set_mat4f_uniform_data(GLuint program, char const* name, GLfloat const* data) {
	GLint location = glGetUniformLocation(program, name);
	if (location < 0) {
		return -1;
	}
	glUniformMatrix4fv(location, 1, GL_FALSE, data);
}
