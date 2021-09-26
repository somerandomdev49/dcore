#version 410 core

in vec3 s_TexCoord;

out vec4 o_Color;
uniform sampler2D u_Tex_Mapper;
uniform sampler2D u_Tex_Main;
uniform sampler2D u_Tex_0;
uniform sampler2D u_Tex_1;
uniform sampler2D u_Tex_2;

uniform float u_Tiling = 10.f;

void main() {
    vec2 texCoord = vec2(s_TexCoord) * u_Tiling;

    vec4 mapperColor = texture(u_Tex_Mapper, vec2(s_TexCoord));
    vec4 colorMain = texture(u_Tex_Main, texCoord) * (1 - mapperColor.r - mapperColor.g - mapperColor.b);
    vec4 color0 = texture(u_Tex_0, texCoord) * mapperColor.r;
    vec4 color1 = texture(u_Tex_1, texCoord) * mapperColor.g;
    vec4 color2 = texture(u_Tex_2, texCoord) * mapperColor.b;
    o_Color = colorMain + color0 + color1 + color2;
    
    // o_Color = texture(u_Tex_Main, s_TexCoord * u_Tiling);
    // o_Color = vec4(0, 1, 0.25, 1);
}
