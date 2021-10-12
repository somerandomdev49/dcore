#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

out vec2 s_TexCoord;

uniform mat4 u_Transform;

void main() {
    s_TexCoord = a_TexCoord;
    gl_Position = u_Transform * vec4(a_Position, 1);
}
