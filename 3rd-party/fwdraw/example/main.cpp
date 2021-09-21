#define FWDRAW_IMPL
// #include <fwdraw.hpp>
// #include <cstdio>
// #include <array>
// #include <iostream>
// #include <glm/ext.hpp>
// #include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include "application.hpp"

// #define TERRAIN_SIZE 1.f
// #define TERRAIN_VERTEX_COUNT 50
// void generateTerrain(fwdraw::Mesh &mesh)
// {
//     int count = TERRAIN_VERTEX_COUNT * TERRAIN_VERTEX_COUNT;
//     std::vector<fwdraw::Vertex> vertices(count);
//     std::vector<fwdraw::Index> indices(count * 6);

//     for(int i = 0; i < TERRAIN_VERTEX_COUNT; ++i)
//     for(int j = 0; j < TERRAIN_VERTEX_COUNT; ++j)
//     {
//         float uvMul = 1.f / (TERRAIN_VERTEX_COUNT - 1);

//         vertices[i + TERRAIN_VERTEX_COUNT * j] = fwdraw::Vertex
//         {
//             /* position */ { TERRAIN_SIZE * i, 0, TERRAIN_SIZE * j },
//             /* normal */ { 0.0f, 1.0f, 0.0f },
//             /* texcoord */ { i * uvMul, j * uvMul, 0 },
//         };
//     }

//     int index = 0;
//     for(int i = 0; i < TERRAIN_VERTEX_COUNT - 1; ++i)
//     for(int j = 0; j < TERRAIN_VERTEX_COUNT - 1; ++j)
//     {
//         // int offset = i * (TERRAIN_VERTEX_COUNT - 1) + j;
//         // indices[index] = (unsigned int)(offset + 0);
//         // indices[index + 1] = (unsigned int)(offset + 1);
//         // indices[index + 2] = (unsigned int)(offset + TERRAIN_VERTEX_COUNT);
//         // indices[index + 3] = (unsigned int)(offset + 1);
//         // indices[index + 4] = (unsigned int)(offset + TERRAIN_VERTEX_COUNT + 1);
//         // indices[index + 5] = (unsigned int)(offset + TERRAIN_VERTEX_COUNT);
//         // index += 6;

//         int row1 = j * TERRAIN_VERTEX_COUNT;
//         int row2 = (j+1) * TERRAIN_VERTEX_COUNT;

//         // triangle 1
//         indices.push_back(row1 + i);
//         indices.push_back(row1 + i + 1);
//         indices.push_back(row2 + i + 1);

//         // triangle 2
//         indices.push_back(row1 + i);
//         indices.push_back(row2 + i + 1);
//         indices.push_back(row2 + i);
//     }

//     // vertices.clear();
//     // indices.clear();

//     // float z = 0.f;

//     // vertices.push_back(fwdraw::Vertex
//     // {
//     //     /* position */ { -0.75, -0.75, z },
//     //     /* normal */ { 0.0f, 0.0f, -1.0f },
//     //     /* texcoord */ { 0, 0 },
//     // });

//     // vertices.push_back(fwdraw::Vertex
//     // {
//     //     /* position */ { +0.75, -0.75, z },
//     //     /* normal */ { 1, 0, -1.0f },
//     //     /* texcoord */ { 1, 0 },
//     // });

//     // vertices.push_back(fwdraw::Vertex
//     // {
//     //     /* position */ { 0.f, +0.75, z },
//     //     /* normal */ { 0.5f, 1 -1.0f },
//     //     /* texcoord */ { 0.5f, 1 },
//     // });

//     // indices.push_back(0);
//     // indices.push_back(1);
//     // indices.push_back(2);

//     mesh.init(vertices, indices);
// }

// void printMatrix(const glm::mat4 &m)
// {
//     std::printf("{ %.2f, %.2f, %.2f, %.2f  \n", m[0].x, m[0].y, m[0].z, m[0].w);
//     std::printf(", %.2f, %.2f, %.2f, %.2f  \n", m[1].x, m[1].y, m[1].z, m[1].w);
//     std::printf(", %.2f, %.2f, %.2f, %.2f  \n", m[2].x, m[2].y, m[2].z, m[2].w);
//     std::printf(", %.2f, %.2f, %.2f, %.2f }\n", m[3].x, m[3].y, m[3].z, m[3].w);
// }

// struct Transform { glm::vec3 pos; glm::quat rot; glm::vec3 scl; };
// glm::mat4 transformMatrix(const Transform &tr)
// {
//     auto t = glm::translate(glm::mat4(1.f), tr.pos);
//     auto r = glm::mat4_cast(tr.rot);
//     auto s = glm::scale(glm::mat4(1.f), tr.scl);
//     return t * r * s;
// }

class Camera
{
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    glm::mat4 mat;
public:
    void LookAt(const glm::vec3 &target)
    {
        mat = glm::lookAt(position, target, up);
    }
};

class MainProgram : Program
{
    fwdraw::Mesh *wallMesh;
    fwdraw::Texture *wallTexture;
public:
    glm::ivec2 GetSize() override { return glm::ivec2(800, 600); }

    void OnStart()
    {
        wallMesh = new fwdraw::Mesh("data/mesh/stonewall.obj");
        wallTexture = new fwdraw::Texture("data/tex/terrain_1.jpg");
    }

    void OnEnd()
    {
        wallMesh->deinit(); delete wallMesh;
        wallTexture->deinit(); delete wallTexture;
    }

    void OnTick(float delta)
    {
        
    }
};

int main(int argc, char *argv[])
{
    // auto i1 = glm::identity<glm::mat4>();
    // printMatrix(i1); std::cout << '\n';
    // // glm::mat4 i2 = { 0, 1, 4, 1,
    // //                     9, 8, 1, 4,
    // //                     2, 1, 2, 3,
    // //                     4, 1, 2, 3 };
    // // printMatrix(i2); std::cout << '\n';
    // // auto m1 = i1 * i2;
    // // printMatrix(m1); std::cout << '\n';
    // // auto m2 = glm::translate<float>({ 0.f, 0.f, 0.f });
    // // printMatrix(m2); std::cout << '\n';
    // // auto m3 = glm::translate<float>({ 0.f, .1f, 0.f });
    // // printMatrix(m3); std::cout << '\n';

    // // return 0;

    // std::puts("main()!");
    // fwdraw::Frame frame({ 800, 600 });
    // frame.init();
    // std::puts("Initialized Frame!");
    // fwdraw::Renderer renderer;

    // fwdraw::Shader shaderObject("data/shader/object.vert", "data/shader/object.frag");
    // fwdraw::Shader shaderTerrain("data/shader/terrain.vert", "data/shader/terrain.frag");

    // fwdraw::Mesh terrainMesh; generateTerrain(terrainMesh);
    // fwdraw::Mesh treeMesh("data/mesh/tree_birch_nocurve.obj");
    // fwdraw::Mesh wallMesh("data/mesh/stonewall.obj");
    // // fwdraw::Mesh cubeMesh("data/mesh/cube.obj");

    // std::vector<fwdraw::Texture> terrainTextures;
    // terrainTextures.push_back(fwdraw::Texture("data/tex/terrain_main.png"));
    // terrainTextures.push_back(fwdraw::Texture("data/tex/terrain_0.jpg"));
    // terrainTextures.push_back(fwdraw::Texture("data/tex/terrain_1.jpg"));
    // terrainTextures.push_back(fwdraw::Texture("data/tex/terrain_2.jpg"));
    // fwdraw::Texture blendMap("data/tex/terrain_blendmap2.png");

    // const char *filenames[] = { "data/tex/bark.jpg", "data/tex/canopy.png" };
    // fwdraw::TextureArray treeTextures(2, filenames);

    // fwdraw::Uniform terrainShader_transformUniform(&shaderTerrain, "u_Transform");
    // fwdraw::Uniform objectShader_transformUniform(&shaderObject, "u_Transform");

    // renderer.bind_shader(&shaderTerrain);
    // {
    //     fwdraw::Uniform(&shaderTerrain, "u_Tex_Mapper").set(0);
    //     fwdraw::Uniform(&shaderTerrain, "u_Tex_Main").set(1);
    //     fwdraw::Uniform(&shaderTerrain, "u_Tex_0").set(2);
    //     fwdraw::Uniform(&shaderTerrain, "u_Tex_1").set(3);
    //     fwdraw::Uniform(&shaderTerrain, "u_Tex_2").set(4);
    // }

    // renderer.bind_shader(&shaderObject);
    // {
    //     fwdraw::Uniform(&shaderObject, "u_Texs").set(0);
    // }

    // glm::vec3 position(0.f, 4.f, 10.f);

    // glm::mat4 view; // = glm::transl(glm::vec3{ 0.f, 0.f, 0.f }); // glm::iden<glm::mat4>();
    //     // glm::lookat(glm::vec3{ 0.f, -1.f, 0.f }, glm::vec3{ 0.f, 0.f, 0.f }, glm::vec3{ 0.f, 1.f, 0.f });

    // glm::mat4 projection = glm::perspective(glm::pi<float>() * 0.35f, 4.f / 3.f, 0.1f, 100.f);
    // glm::mat4 transformTerrain, transformTree;
    
    // glm::vec3 cameraDir = glm::normalize(glm::vec3(0.f, 0.f, -1.f));
    // const auto calcTransform = [&]()
    // {
    //     view = glm::lookAt(
    //         position,
    //         position + cameraDir,
    //         glm::vec3(0.f, 1.f, 0.f)
    //     );

    //     transformTree = projection * view; // glm::transl(glm::vec3{0, 1, 0}) * 
    //     transformTerrain = projection * view;
    // };

    // calcTransform();

    // bool wireframeMode = false;
    // float wireframeModeTimeout = 0.f;

    // const auto processInput = [&](float deltaTime)
    // {
    //     static float speed = 6.f, rotSpeed = 1.5f;
    //     glm::vec3 move = {0, 0, 0}; bool changed = false;
    //     if(frame.key_pressed(GLFW_KEY_A))
    //         move = move + glm::normalize(glm::cross(glm::vec3(0, 1, 0), cameraDir)) * speed * deltaTime;
    //     if(frame.key_pressed(GLFW_KEY_D))
    //         move = move - glm::normalize(glm::cross(glm::vec3(0, 1, 0), cameraDir)) * speed * deltaTime;
        
    //     if(frame.key_pressed(GLFW_KEY_W))
    //         move = move + cameraDir * speed * deltaTime;
    //     if(frame.key_pressed(GLFW_KEY_S))
    //         move = move - cameraDir * speed * deltaTime;
        
    //     if(frame.key_pressed(GLFW_KEY_LEFT_SHIFT))
    //         move.y -= speed * deltaTime;
    //     if(frame.key_pressed(GLFW_KEY_SPACE))
    //         move.y += speed * deltaTime;

    //     if(frame.key_pressed(GLFW_KEY_Q))
    //     {
    //         cameraDir = cameraDir * glm::angleAxis(-rotSpeed * deltaTime, glm::vec3(0, 1, 0));
    //         changed = true;
    //     }

    //     if(frame.key_pressed(GLFW_KEY_E))
    //     {
    //         cameraDir = cameraDir * glm::angleAxis(+rotSpeed * deltaTime, glm::vec3(0, 1, 0));
    //         changed = true;
    //     }

    //     if(changed || move != glm::vec3(0, 0, 0))
    //     {
    //         position = position + move;
    //         calcTransform();
    //     }
    // };

    // // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // while(!frame.should_end())
    // {
    //     frame.on_begin();
    //     float deltaTime = frame.delta();

    //     if(wireframeModeTimeout >= 0.2f && frame.key_pressed(GLFW_KEY_B))
    //     {
    //         wireframeModeTimeout = 0;
    //         wireframeMode = !wireframeMode;
    //         if(wireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //         else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //     } else
    //     {
    //         wireframeModeTimeout += deltaTime;
    //     }

    //     processInput(deltaTime);

    //     renderer.begin();

    //     {
    //         renderer.bind_shader(&shaderTerrain);
    //         terrainShader_transformUniform.set(transformTerrain);

    //         renderer.bind_texture(0, &blendMap);
    //         for(int i = 0; i < terrainTextures.size(); ++i)
    //             renderer.bind_texture(i + 1, &terrainTextures[i]);

    //         renderer.render_mesh(&terrainMesh);
    //     }
    //     {
    //         renderer.bind_shader(&shaderObject);
    //         objectShader_transformUniform.set(transformTree);
    //         renderer.bind_texture(0, &treeTextures);
    //         renderer.render_mesh(&treeMesh);
    //     }

    //     frame.on_end();
    // }

    // shaderObject.deinit();
    // shaderTerrain.deinit();
    // // for(int i = 0; i < 4; ++i)
    // //     terrainTextures[i].deinit();
    // for(auto t : terrainTextures) t.deinit();
    // blendMap.deinit();
    // treeMesh.deinit();
    // terrainMesh.deinit();
    // renderer.deinit();
    // frame.deinit();
}
