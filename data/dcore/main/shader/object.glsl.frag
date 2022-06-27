#version 410 core

in vec4 s_Position;
in vec2 s_TexCoord;
in vec3 s_Normal;

out vec4 o_Color;
uniform sampler2D u_Tex;
uniform vec3 u_FogColor;

uniform float u_Tiling = 10.f;
const vec3 SUN = normalize(vec3(1, 1, 0));
const float zFogNear = 40.0 + 120.0;
const float zFogFar  = 80.0 + 180.0;

void main() {
    vec4 c = texture(u_Tex, u_Tiling * s_TexCoord); 
    if(c.a < 0.1) discard;
	float shadow = clamp(dot(s_Normal, SUN), 0.4, 1.0);
	c *= vec4(vec3(shadow), 1.0);

	float dist = length(s_Position.xyz);
	float fogAmount = (zFogFar - dist) / (zFogFar - zFogNear);
	fogAmount = clamp(fogAmount, 0.0, 1.0);
    o_Color = mix(vec4(u_FogColor, 1), c, fogAmount);
}
