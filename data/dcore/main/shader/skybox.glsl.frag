#version 410 core

out vec4 o_Color;

in vec3 s_TexCoord;

uniform samplerCube u_Skybox;

void main()
{    
    o_Color = texture(u_Skybox, s_TexCoord);
}
