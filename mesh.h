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
void setNormal(std::vector<Vertex> &vertices,
               unsigned int v1, unsigned int v2, unsigned int v3);
void setIndex(std::vector<unsigned int> &indices, unsigned int r3, unsigned int r2,
              unsigned int r1, unsigned int l3, unsigned int l2, unsigned int l1);
void setIndices(std::vector<unsigned int> &indices, std::vector<Vertex> &vertices,
                int worldHeight, int worldWidth);
glm::vec3 setColor(float pos, int biome);
Mesh branch(glm::vec3 startVertex = glm::vec3(0, 0, 0), float scale = 1, bool leaf = false);
std::vector<Mesh> genFlora(glm::vec3 startVertexPosition);
void setWorldVertices(std::vector<Vertex> &vertices,
                      int worldHeight, int worldWidth,
                      int biome, int seed, float frequency,
                      float lacunarity, float gain,
                      int octaves, int scale);
Mesh genWorld(int worldHeight, int worldWidth,
              int biome, int seed, float frequency,
              float lacunarity, float gain,
              int octaves, int scale);
Mesh genSkyBox(int worldHeight, int worldWidth);
Mesh genLightSource();
#endif