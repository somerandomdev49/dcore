#version 410 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec2 a_TexCoord;
out vec2 s_TexCoord;

uniform mat2 u_Transform;

void main() {
    s_TexCoord = a_TexCoord;
    gl_Position = vec4(u_Transform * a_Position, 0, 1);
}

