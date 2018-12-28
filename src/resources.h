#pragma once

#include "graphics.h"

struct graphics_resources {
	struct shader_info shaders[128];
	char const* shader_names[128];

	struct draw_info draw_infos[32];
	char const* draw_info_names[32];

	struct texture_info textures[128];
	char const* texture_names[128];
};

struct shader_info* graphics_resources_register_shader(struct graphics_resources* res, struct shader_info shader, char const* name);
struct draw_info* graphics_resources_register_draw_info(struct graphics_resources* res, struct draw_info info, char const* name);
struct texture_info* graphics_resources_register_texture(struct graphics_resources* res, struct texture_info info, char const* name);

void graphics_resources_destroy_shader_named(struct graphics_resources* res, char const* name);
void graphics_resources_destroy_shader(struct graphics_resources* res, struct shader_info* shader);
void graphics_resources_destroy_draw_info_named(struct graphics_resources* res, char const* name);
void graphics_resources_destroy_draw_info(struct graphics_resources* res, struct draw_info* draw);
void graphics_resources_destroy_texture_named(struct graphics_resources* res, char const* name);
void graphics_resources_destroy_texture(struct graphics_resources* res, struct texture_info* texture);

struct shader_info* graphics_resources_get_shader_named(struct graphics_resources* res, char const* name);
struct draw_info* graphics_resources_get_draw_info_named(struct graphics_resources* res, char const* name);
struct texture_info* graphics_resources_get_texture_named(struct graphics_resources* res, char const* name);
