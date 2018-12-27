#version 150

in vec2 f_uv;

uniform sampler2D tex;

out vec4 out_color;

void main() {
	out_color = texture(tex, f_uv);
}

