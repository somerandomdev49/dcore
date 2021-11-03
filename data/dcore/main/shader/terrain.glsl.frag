#version 410 core

in vec4 s_Position;
in vec3 s_Normal;
in vec2 s_TexCoord;

out vec4 o_Color;
uniform sampler2D u_BlendMap;
uniform sampler2D u_Tex0;
uniform sampler2D u_TexR;
uniform sampler2D u_TexG;
uniform sampler2D u_TexB;

uniform float u_Tiling = 10.f;

const vec3 SUN = normalize(vec3(1, -1, 0));
const vec4 FOG_COLOR = vec4(0.2, 0.4, 0.5, 1.0);

const float zNear = 0.1;
const float zFar = 100.0;

const float zFogNear = 40.0 + 40.0;
const float zFogFar  = 80.0 + 40.0;

float calculateFogAmount(float z) { return (zFogFar - z) / (zFogFar - zFogNear); }
// float linearDepth(vec4 clipSpacePos)
// {
//     float ndcDepth = clipSpacePos.z / clipSpacePos.w;
//     return (((zFar - zNear) * ndcDepth) + zNear + zFar) / 2.0;
// }

void main()
{
    // vec2 texCoord = s_TexCoord * u_Tiling;

    // vec4 mapperColor = texture(u_Tex_Mapper, s_TexCoord);
    // vec4 colorMain = texture(u_Tex_Main, texCoord) * (1 - mapperColor.r - mapperColor.g - mapperColor.b);
    // vec4 color0 = texture(u_Tex_0, texCoord) * mapperColor.r;
    // vec4 color1 = texture(u_Tex_1, texCoord) * mapperColor.g;
    // vec4 color2 = texture(u_Tex_2, texCoord) * mapperColor.b;
    // o_Color = colorMain + color0 + color1 + color2;
    
    o_Color = texture(u_Tex0, s_TexCoord * u_Tiling);

    // o_Color = vec4(0.0, 1.0, 0.25, 1.0);
    float shadow = dot(s_Normal, SUN);
    o_Color *= vec4(vec3(shadow), 1.0);

    float dist = length(s_Position.xyz);
    float fogAmount = (zFogFar - dist) / (zFogFar - zFogNear);

    //calculateFogAmount(linearDepth(s_Position));
    fogAmount = clamp(fogAmount, 0.0, 1.0);
    o_Color = mix(FOG_COLOR, o_Color, fogAmount);
}
