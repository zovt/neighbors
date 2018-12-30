#include <string.h>

#include "resources.h"
#include "util.h"


#define impl_graphics_resources_register(type, type_name, info_name, names_name)\
type* graphics_resources_register_ ## type_name (struct graphics_resources* res, type to_reg, char const* name) {\
	size_t name_len = strlen(name);\
	for (size_t i = 0; i < sizeof(res->info_name)/sizeof(res->info_name[0]); ++i) {\
		if (res->names_name[i] == NULL) {\
			res->names_name[i] = name;\
			res->info_name[i] = to_reg;\
			return &res->info_name[i];\
		}\
		char const* other_name = res->names_name[i];\
		size_t other_name_len = strlen(other_name);\
		if (strncmp(name, other_name, (name_len < other_name_len ? name_len : other_name_len)) == 0) {\
			p_err("type resource already exists with name %s\n", name);\
			return NULL;\
		}\
	}\
\
	return NULL;\
}

impl_graphics_resources_register(struct shader_info, shader, shaders, shader_names)
impl_graphics_resources_register(struct draw_info, draw_info, draw_infos, draw_info_names)
impl_graphics_resources_register(struct texture_info, texture, textures, texture_names)

#define impl_graphics_resources_destroy(type, type_name, info_field, names_field, destroy_fn_name)\
void graphics_resources_destroy_##type_name##_named(struct graphics_resources* res, char const* name) {\
	size_t name_len = strlen(name);\
	for (size_t i = 0; i < sizeof(res->info_field)/sizeof(res->info_field[0]); ++i) {\
		char const* other_name = res->names_field[i];\
		size_t other_name_len = strlen(other_name);\
		if (strncmp(name, other_name, (name_len < other_name_len ? name_len : other_name_len)) == 0) {\
			res->names_field[i] = NULL;\
			destroy_fn_name(&res->info_field[i]);\
			return;\
		}\
	}\
	p_warn("Didn't find a type_name named %s\n", name);\
}\
void graphics_resources_destroy_##type_name(struct graphics_resources* res, type * to_destroy) {\
	for (size_t i = 0; i < sizeof(res->info_field)/sizeof(res->info_field[0]); ++i) {\
		type* other = &res->info_field[i];\
		if (other == to_destroy) {\
			res->names_field[i] = NULL;\
			destroy_fn_name(&res->info_field[i]);\
			return;\
		}\
	}\
	p_warn("Didn't find anything to destroy. Maybe an unregistered pointer?\n");\
}

impl_graphics_resources_destroy(struct shader_info, shader, shaders, shader_names, shader_info_destroy)
impl_graphics_resources_destroy(struct draw_info, draw_info, draw_infos, draw_info_names, draw_info_destroy)
impl_graphics_resources_destroy(struct texture_info, texture, textures, texture_names, texture_info_destroy)

#define impl_graphics_resources_get(type, type_name, info_field, name_field)\
type * graphics_resources_get_##type_name##_named(struct graphics_resources* res, char const* name) {\
	size_t name_len = strlen(name);\
	for (size_t i = 0; i < sizeof(res->info_field)/sizeof(res->info_field[0]); ++i) {\
		const char* other_name = res->name_field[i];\
		size_t other_len = strlen(other_name);\
		if (strncmp(name, other_name, (name_len < other_len ? name_len : other_len)) == 0) {\
			return &res->info_field[i];\
		}\
	}\
\
	return NULL;\
}

impl_graphics_resources_get(struct shader_info, shader, shaders, shader_names)
impl_graphics_resources_get(struct draw_info, draw_info, draw_infos, draw_info_names)
impl_graphics_resources_get(struct texture_info, texture, textures, texture_names)
