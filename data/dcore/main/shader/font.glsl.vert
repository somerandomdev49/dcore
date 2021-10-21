#version 410 core

const vec2 POS[] = vec2[4](vec2(-1, 1), vec2(1, 1), vec2(-1, -1), vec2(1, -1));

uniform vec2 u_TexCoords[4];
uniform mat3 u_Transform;

out vec2 s_TexCoord;

void main() {
    s_TexCoord = u_TexCoords[gl_VertexID];
    gl_Position = vec4(u_Transform * vec3(POS[gl_VertexID], 0), 1); // 
}

