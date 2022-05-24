#version 460 core

in vec2 vTexcoord;
out vec4 fColor;

uniform sampler2D u_texture;

void main() {
	fColor = texture(u_texture, vTexcoord);
}
