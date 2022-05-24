#version 460 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec2 aTexcoord;
layout (location = 2) in vec4 aOffset;

uniform mat4 u_mvp;

out vec2 vTexcoord;

void main() {
    gl_Position = u_mvp * (aPos + aOffset);
    vTexcoord = aTexcoord;
}