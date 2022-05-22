#version 460 core

in vec4 vColor;
//in vec2 v_texcoord;
out vec4 fColor;

//uniform sampler2D u_texture;

void main() {
	fColor = vColor;
//	fColor = texture(u_texture, v_texcoord);
}
