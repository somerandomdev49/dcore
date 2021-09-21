#version 410 core

in vec3 s_TexCoord;

out vec4 o_Color;
uniform sampler2DArray u_Texs;

uniform float u_Tiling = 1.f;

void main() {
    vec4 c = texture(u_Texs, vec3(vec2(s_TexCoord) * u_Tiling, s_TexCoord.z));
    if(c.a < 0.1) discard;
    o_Color = c; // vec4(1, 0.1, 0.3, 1); // 
}
