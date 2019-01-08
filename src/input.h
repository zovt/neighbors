#pragma once

#include "game.h"

typedef void (*key_handler_fn)(struct entity*, int, int, int);
struct controller {
	size_t entity_id;
	key_handler_fn handler;
};

void controller_handle_input(struct controller* controller, struct world* world, int key, int action, int mods);

void change_position(struct entity* entity, int key, int action, int mods);
