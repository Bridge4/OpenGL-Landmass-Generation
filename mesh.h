#ifndef MESH_CLASS_H
#define MESH_CLASS_H
#include "CSCIx229.h"
//source: https://learnopengl.com/Model-Loading/Mesh

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec3 Normal;
    //glm::vec2 TexCoords;
};

struct Texture
{
    unsigned int id;
    std::string type;
};

class Mesh
{
public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    void Draw(int primType=1);

private:
    //  render data
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};

#endif