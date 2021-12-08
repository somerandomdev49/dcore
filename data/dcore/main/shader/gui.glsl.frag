#version 410 core

in vec2 s_TexCoord;

out vec4 o_Color;
uniform sampler2D u_Tex;
uniform vec4 u_Color = vec4(1.0, 1.0, 1.0, 1.0);

void main() {
    vec4 c = texture(u_Tex, s_TexCoord);
    o_Color = c * u_Color;
}
