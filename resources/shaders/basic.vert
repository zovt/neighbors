#version 150

in vec3 position;
in vec2 uv; // unused for now

uniform mat4 proj;
uniform mat4 world;
uniform mat4 transform;

out vec2 f_uv;

void main() {
	gl_Position = proj * world * transform * vec4(position, 1.0);
	f_uv = uv;
}
