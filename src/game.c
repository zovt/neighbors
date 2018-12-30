#include <stdlib.h>

#include "game.h"
#include "util.h"

struct entity* find_entity_id(struct world* world, size_t entity_id) {
	for (size_t i = 0; i < sizeof(world->entities)/sizeof(world->entities[0]); i++) {
		struct entity* entity = &world->entities[i];
		if (entity->id == entity_id) {
			return entity;
		}
	}
	return NULL;
}

struct entity* entity_create(struct world* world) {
	struct entity* slot = find_entity_id(world, 0);
	if (slot == NULL) {
		p_err("Out of entity space!");
		exit(1);
	}

	slot->id = world->last_entity_id + 1;
	world->last_entity_id = slot->id;

	return slot;
}

void entity_destroy(struct world* world, struct entity* entity) {
	struct entity* slot = find_entity_id(world, entity->id);
	if (slot == NULL) {
		p_err("This entity is not registered with the world: %ld\n", entity->id);
	}
	if (slot != entity) {
		p_err("Duplicate entity ID detected: %ld\n", entity->id);
	}
	slot->id = 0;
}

struct entity* entity_get(struct world* world, size_t entity_id) {
	for (size_t i = 0; i < WORLD_ENTITIES; ++i) {
		struct entity* entity = &world->entities[i];
		if (entity->id == entity_id) {
			return entity;
		}
	}

	return NULL;
}
