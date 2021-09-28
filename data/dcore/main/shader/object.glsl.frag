#version 410 core

in vec3 s_TexCoord;

out vec4 o_Color;
uniform sampler2D u_Tex;

uniform float u_Tiling = 10.f;

void main() {
    vec4 c = texture(u_Tex, vec2(s_TexCoord) * u_Tiling);
    if(c.a < 0.1) discard;
    o_Color = c;
}
