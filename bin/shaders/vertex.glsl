#version 460 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 u_mvp;

out vec4 vColor;

void main() {
    gl_Position = u_mvp * aPos;
    vColor = aColor;
}