#include <string.h>

#include "util.h"
#include "graphics.h"

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
	
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		render(window, &sprite, &basic);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

cleanup:
	draw_info_destroy(&sprite);
	shader_info_destroy(&basic);
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
