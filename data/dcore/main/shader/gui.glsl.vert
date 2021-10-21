#version 410 core

const vec2 POS[] = vec2[4](vec2(-1, 1), vec2(1, 1), vec2(-1, -1), vec2(1, -1));
const vec2 TEX[] = vec2[4](vec2(0, 1), vec2(1, 1), vec2(0, 0), vec2(1, 0));
out vec2 s_TexCoord;

uniform mat2 u_Transform;

void main() {
    s_TexCoord = TEX[gl_VertexID];
    gl_Position = vec4(POS[gl_VertexID], 0, 1); // u_Transform * 
}

