#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct draw_info {
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLint n_indices;
};

static GLuint const POS_ATTRIB = 0;
static GLuint const UV_ATTRIB = 1;

struct draw_info _draw_info_create();
void draw_info_destroy(struct draw_info* info);
void draw_info_activate(struct draw_info const* info);
void draw_info_draw(struct draw_info const* info);

// TODO: Parameterize this over UV
struct draw_info sprite_create();

struct shader_info {
	GLuint vert;
	GLuint frag;
	GLuint program;
};

struct shader_info _shader_info_create();
void shader_info_destroy(struct shader_info* info);
void shader_info_activate(struct shader_info const* info);

// Be sure to check error buf
struct shader_info basic_shader_create(char* err_buf, size_t err_buf_sz);

void set_mat4f_uniform_data(GLuint program, char const* name, GLfloat const* data);

void render(GLFWwindow const* window, struct draw_info const* sprite, struct shader_info* basic);

