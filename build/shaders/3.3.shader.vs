#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aColor;

out vec3 vertexColor;
uniform vec2 positionOffset;

void main() {
    gl_Position = vec4(aPos, 1.0) + vec4(positionOffset,0.0,0.0);
    vertexColor = aColor + vec3(positionOffset, 0.0);
}