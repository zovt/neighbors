#include <string.h>
#include <cglm/cglm.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "util.h"
#include "graphics.h"
#include "resources.h"
#include "input.h"
#include "resources/textures/test.png.h"


mat4 proj = {0};

void window_size_cb(GLFWwindow* _window, int width, int height) {
	glm_ortho(0, width, 0, height, -1.0f, 1.f, proj);
	glViewport(0, 0, width, height);
}

struct world* current_world;
struct controller* current_controller;
void key_cb(GLFWwindow* _window, int key, int _sc, int action, int mods) {
	controller_handle_input(current_controller, current_world, key, action, mods);
}

int main(int argc, char const* const argv) {
	if (!glfwInit()) {
		p_err("Failed to initialize GLFW\n");
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	GLFWwindow* window = glfwCreateWindow(640, 480, "Neighbors", NULL, NULL);
	if (!window) {
		p_err("Failed to create window\n");
		return 1;
	}

	glfwMakeContextCurrent(window);

	// register callbacks
	glfwSetWindowSizeCallback(window, window_size_cb);
	glfwSetKeyCallback(window, key_cb);

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEBUG_OUTPUT);

	struct draw_info sprite = sprite_create();

	char err_buf[512] = {0};
	struct shader_info basic = basic_shader_create(err_buf, sizeof(err_buf));
	if (strlen(err_buf) != 0) {
		p_err("Failed to create shader: %s\n", err_buf);
		goto cleanup;
	}

	mat4 world = {0};
	glm_mat4_identity(world);
	struct uniform_mat4f world_uni = {
		.name = "world",
		.data = world[0],
	};

	glm_ortho(0, 640, 0, 480, -1.0f, 1.f, proj);
	glViewport(0, 0, 640, 480);

	struct uniform_mat4f proj_uni = {
		.name = "proj",
		.data = proj[0],
	};

	struct texture_info test = texture_info_stbi_load_memory(test_png, test_png_len);
	struct graphics_resources g_res = {0};
	graphics_resources_register_texture(&g_res, test, "test");
	graphics_resources_register_shader(&g_res, basic, "basic");
	graphics_resources_register_draw_info(&g_res, sprite, "sprite");

	struct world world_info = {0};
	struct entity* player = entity_create(&world_info);
	player->x_pos = 100;
	player->y_pos = 40;
	player->width = 80;
	player->height = 100;

	current_world = &world_info;
	struct controller player_controller = {
		.entity_id = player->id,
		.handler = change_position,
	};
	current_controller = &player_controller;

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		render(window, &world_info, &g_res, proj_uni, world_uni);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

cleanup:
	draw_info_destroy(&sprite);
	shader_info_destroy(&basic);
	texture_info_destroy(&test);
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
