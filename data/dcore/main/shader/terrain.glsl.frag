#version 410 core

in vec4 s_Position;
in vec3 s_Normal;
in vec2 s_TexCoord;

out vec4 o_Color;
uniform vec3 u_FogColor;
uniform sampler2D u_BlendMap;
uniform sampler2D u_Tex0;
uniform sampler2D u_TexR;
uniform sampler2D u_TexG;
uniform sampler2D u_TexB;

uniform float u_Tiling = 10.f;

const vec3 SUN = normalize(vec3(1, 1, 0));
// const vec4 FOG_COLOR = ;

const float zNear = 0.1;
const float zFar = 100.0;

const float zFogNear = 40.0 + 120.0;
const float zFogFar  = 80.0 + 180.0;

float calculateFogAmount(float z) { return (zFogFar - z) / (zFogFar - zFogNear); }
// float linearDepth(vec4 clipSpacePos)
// {
//     float ndcDepth = clipSpacePos.z / clipSpacePos.w;
//     return (((zFar - zNear) * ndcDepth) + zNear + zFar) / 2.0;
// }


vec3 hash3(vec2 p)
{
	vec3 q = vec3(dot(p,vec2(127.1,311.7)), 
				  dot(p,vec2(269.5,183.3)), 
				  dot(p,vec2(419.2,371.9)));
	return fract(sin(q) * 43758.5453);
}

float voronoise(in vec2 p)
{
	float k = 1.984375; // 1.0 + 63.0 * pow(0.5, 6.0);

	vec2 i = floor(p);
	vec2 f = fract(p);
	
	vec2 a = vec2(0.0,0.0);
	for(int y = -2; y <= 2; y++)
	for(int x = -2; x <= 2; x++)
	{
		vec2  g = vec2(x, y);
		vec3  o = hash3(i + g) * vec3(0.5, 0.5 ,1.0);
		vec2  d = g - f + o.xy;
		float w = pow(1.0 - smoothstep(0.0, 1.414, length(d)), k);
		a += vec2(o.z * w, w);
	}
	
	return a.x / a.y;
}

// vec4 quadSample(in vec2 uv, float m, sampler2D tex)
// {
// 	vec4 color0 = texture(tex, vec2(0 + uv.x * m, 0 + uv.y * m));
// 	vec4 color1 = texture(tex, vec2(1 - uv.x * m, 0 + uv.y * m));
// 	vec4 color2 = texture(tex, vec2(0 + uv.x * m, 1 - uv.y * m));
// 	vec4 color3 = texture(tex, vec2(1 - uv.x * m, 1 - uv.y * m));
// 	return mix(mix(color0, color1, uv.x), mix(color2, color3, 1 - uv.x), 1 - uv.y);
// }

vec2 rotated(in vec2 r, float a)
{
	return vec2(r.x * cos(a) - r.y * sin(a), r.x * sin(a) + r.y * cos(a));
}

mat3 makeRotation(float angleInRadians)
{
    float c = cos(angleInRadians);
    float s = sin(angleInRadians);
    mat3 m = mat3(c, -s, 0, s, c, 0, 0, 0, 1);
    return m;
}

void main()
{
	vec2 texCoord = s_TexCoord * u_Tiling;

	// vec4 mapperColor = texture(u_Tex_Mapper, s_TexCoord);
	// vec4 colorMain = texture(u_Tex_Main, texCoord) * (1 - mapperColor.r - mapperColor.g - mapperColor.b);
	// vec4 color0 = texture(u_Tex_0, texCoord) * mapperColor.r;
	// vec4 color1 = texture(u_Tex_1, texCoord) * mapperColor.g;
	// vec4 color2 = texture(u_Tex_2, texCoord) * mapperColor.b;
	// o_Color = colorMain + color0 + color1 + color2;
const float n = 2;
	vec4 color = texture(u_Tex0, texCoord) / n;
	color += texture(u_Tex0, vec2(makeRotation(0.17) * vec3(texCoord*1.5+0.1, 1))) / n;
	// vec4 color = quadSample(texCoord, 1, u_Tex0);

	// o_Color = vec4(0.0, 1.0, 0.25, 1.0);
	float shadow = clamp(dot(s_Normal / 2 + vec3(0.5), SUN), 0.4, 1.0);
	color *= vec4(vec3(shadow), 1.0);

	float dist = length(s_Position.xyz);
	float fogAmount = (zFogFar - dist) / (zFogFar - zFogNear);
	fogAmount = clamp(fogAmount, 0.0, 1.0);
	o_Color = mix(vec4(u_FogColor, 1), color, fogAmount);
}
