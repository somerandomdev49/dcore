#version 410 core

in vec2 s_TexCoord;

out vec4 o_Color;
uniform sampler2D u_Tex;
uniform vec4 u_Color = vec4(0.75, 0.25, 0.25, 1);

void main() {
    vec4 c = texture(u_Tex, s_TexCoord);
    o_Color = vec4(c.r) ;// mix(vec4(0.8, 0.1, 0.2, 1.0), vec4(c.r), c.r); //vec4(c.r); // vec4(s_TexCoord, 0.0, 1.0); // 
}
