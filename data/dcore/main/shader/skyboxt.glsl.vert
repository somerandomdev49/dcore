#version 410 core
layout(location = 0) in vec3 a_Position;
out vec3 s_TexCoord;
uniform mat4 u_Transform;

void main()
{
    s_TexCoord = a_Position;
    gl_Position = (u_Transform * vec4(a_Position, 1)).xyww;
}

