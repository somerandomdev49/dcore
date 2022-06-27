#version 410 core
out vec4 o_Color;

in vec3 s_TexCoord;

uniform samplerCube u_Skybox1;
uniform samplerCube u_Skybox2;
uniform float u_Alpha;

void main()
{    
    o_Color
        = texture(u_Skybox1, s_TexCoord) * (1 - u_Alpha)
        + texture(u_Skybox2, s_TexCoord) * u_Alpha;
}
