#include <GLFW/glfw3.h>

#include "input.h"

void controller_handle_input(struct controller* controller, struct world* world, int key, int action, int mods) {
	struct entity* entity = entity_get(world, controller->entity_id);
	controller->handler(entity, key, action, mods);
}

void change_position(struct entity* entity, int key, int action, int mods) {
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {
			case GLFW_KEY_UP:
				entity->y_pos += 10;
				break;
			case GLFW_KEY_DOWN:
				entity->y_pos -= 10;
				break;
			case GLFW_KEY_LEFT:
				entity->x_pos -= 10;
				break;
			case GLFW_KEY_RIGHT:
				entity->x_pos += 10;
				break;
		}
	}
}
