#pragma once

#include "game.h"

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

struct texture_info {
	GLuint texture;
};

struct texture_info _texture_info_create();
void texture_info_destroy(struct texture_info* info);
struct texture_info texture_info_stbi_load_memory(unsigned char const* data, size_t len);

struct uniform_mat4f {
	char const* name;
	GLfloat const* data;
};

int uniform_mat4f_enable(GLuint program, struct uniform_mat4f uniform);

struct uniform_texture {
	char const* name;
	struct texture_info const* info;
};

int uniform_texture_enable(GLuint program, struct uniform_texture uniform, GLuint slot);

struct graphics_resources;
void render(GLFWwindow const* window, struct world const* world_info, struct graphics_resources*, struct uniform_mat4f proj, struct uniform_mat4f world);
