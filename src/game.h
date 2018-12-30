#pragma once

#include <stddef.h>

enum entity_type {
	ET_character,
	ET_item,
	ET_trigger,
};

struct entity {
	size_t id;
	enum entity_type type;

	int x_pos;
	int y_pos;
	unsigned int width;
	unsigned int height;
};

#define WORLD_ENTITIES 512
struct world {
	struct entity entities[WORLD_ENTITIES]; // CONSIDER: is this enough?
	size_t last_entity_id;
};

struct entity* entity_create(struct world* world);
void entity_destroy(struct world* world, struct entity* entity);
struct entity* entity_get(struct world* world, size_t entity_id);
