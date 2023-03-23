#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

out vec3 vFragColor;

void main() {
    vFragColor = aVertexColor;
    vec2 transform = vec2(2., 0.5);
    gl_Position = vec4((aVertexPosition + 0.5) * transform, 0, 1);
}