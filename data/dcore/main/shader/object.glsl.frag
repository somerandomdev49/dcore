#version 410 core

in vec2 s_TexCoord;
in vec3 s_Normal;

out vec4 o_Color;
uniform sampler2D u_Tex;

uniform float u_Tiling = 10.f;
const vec3 SUN = normalize(vec3(1, 1, 0));

void main() {
    vec4 c = texture(u_Tex, u_Tiling * s_TexCoord); 
    if(c.a < 0.1) discard;
	float shadow = clamp(dot(s_Normal, SUN), 0.4, 1.0);
	c *= vec4(vec3(shadow), 1.0);
    o_Color = c;
}
