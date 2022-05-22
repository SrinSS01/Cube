#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

in vec2 vTexCoord[];
uniform mat4 u_mvp;
out vec2 v_texcoord;

void emit(vec3 offset, mat4 mvp) {
    gl_Position = mvp * (gl_in[0].gl_Position + vec4(offset, 0));
    EmitVertex();
}

vec2 getTexCoordX1X2(vec2 coord) {
    float u = (coord.x - 1) * coord.y;
    float v = coord.x * coord.y;
    return vec2(u, v);
}

void main() {
    float offset = 50;
    vec2 x1x2 = getTexCoordX1X2(vTexCoord[0]);
    vec2 y1y2 = vec2(0, 1);
    v_texcoord = vec2(x1x2.x, y1y2.x);
    emit(vec3(-offset, -offset, +offset), u_mvp);
    v_texcoord = vec2(x1x2.y, y1y2.x);
    emit(vec3(+offset, -offset, +offset), u_mvp);
    v_texcoord = vec2(x1x2.x, y1y2.y);
    emit(vec3(-offset, +offset, +offset), u_mvp);
    v_texcoord = vec2(x1x2.y, y1y2.y);
    emit(vec3(+offset, +offset, +offset), u_mvp);
    EndPrimitive();

    v_texcoord = vec2(x1x2.x, y1y2.x);
    emit(vec3(+offset, -offset, -offset), u_mvp);
    v_texcoord = vec2(x1x2.y, y1y2.x);
    emit(vec3(-offset, -offset, -offset), u_mvp);
    v_texcoord = vec2(x1x2.x, y1y2.y);
    emit(vec3(+offset, +offset, -offset), u_mvp);
    v_texcoord = vec2(x1x2.y, y1y2.y);
    emit(vec3(-offset, +offset, -offset), u_mvp);
    EndPrimitive();

    v_texcoord = vec2(x1x2.x, y1y2.x);
    emit(vec3(-offset, -offset, -offset), u_mvp);
    v_texcoord = vec2(x1x2.y, y1y2.x);
    emit(vec3(-offset, -offset, +offset), u_mvp);
    v_texcoord = vec2(x1x2.x, y1y2.y);
    emit(vec3(-offset, +offset, -offset), u_mvp);
    v_texcoord = vec2(x1x2.y, y1y2.y);
    emit(vec3(-offset, +offset, +offset), u_mvp);
    EndPrimitive();

    v_texcoord = vec2(x1x2.x, y1y2.x);
    emit(vec3(+offset, -offset, +offset), u_mvp);
    v_texcoord = vec2(x1x2.y, y1y2.x);
    emit(vec3(+offset, -offset, -offset), u_mvp);
    v_texcoord = vec2(x1x2.x, y1y2.y);
    emit(vec3(+offset, +offset, +offset), u_mvp);
    v_texcoord = vec2(x1x2.y, y1y2.y);
    emit(vec3(+offset, +offset, -offset), u_mvp);
    EndPrimitive();

    v_texcoord = vec2(x1x2.x, y1y2.x);
    emit(vec3(-offset, +offset, +offset), u_mvp);
    v_texcoord = vec2(x1x2.y, y1y2.x);
    emit(vec3(+offset, +offset, +offset), u_mvp);
    v_texcoord = vec2(x1x2.x, y1y2.y);
    emit(vec3(-offset, +offset, -offset), u_mvp);
    v_texcoord = vec2(x1x2.y, y1y2.y);
    emit(vec3(+offset, +offset, -offset), u_mvp);
    EndPrimitive();

    v_texcoord = vec2(x1x2.x, y1y2.x);
    emit(vec3(-offset, -offset, -offset), u_mvp);
    v_texcoord = vec2(x1x2.y, y1y2.x);
    emit(vec3(+offset, -offset, -offset), u_mvp);
    v_texcoord = vec2(x1x2.x, y1y2.y);
    emit(vec3(-offset, -offset, +offset), u_mvp);
    v_texcoord = vec2(x1x2.y, y1y2.y);
    emit(vec3(+offset, -offset, +offset), u_mvp);
    EndPrimitive();
}
