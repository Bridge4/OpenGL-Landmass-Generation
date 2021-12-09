#ifndef MESH_CLASS_H
#define MESH_CLASS_H
#include "CSCIx229.h"
//source: https://learnopengl.com/Model-Loading/Mesh

/*
    Code in this file and the corresponding .cpp file is reused from the link above

    The changes I made included removing the texture vec2 from the Vertex struct and adding a color attribute
    
    The setupMesh() function does all the glBindBuffer and glBindVertexArray calls for the VBOs, VAOs and EBOs/IBOs
    
    This code is identical to code I had written earlier in the project but all it does is put the methods into a Mesh class
    which makes it easier to organize and create a large number of objects.

    Since each Mesh has its own VAO, VBO and EBO I don't need to create any global variables to bind which cleans up the main.cpp file

*/
struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec3 Normal;
};

class Mesh
{
public:
    // mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    //draws based on primitive type selected
    void Draw(int primType=1);

private:
    //  render data
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};

//sets normals for the individual triangles in the world mesh
void setNormal(std::vector<Vertex> &vertices,
               unsigned int v1, unsigned int v2, unsigned int v3);
//sets the index of triangles that construct the world mesh
void setIndex(std::vector<unsigned int> &indices, unsigned int r3, unsigned int r2,
              unsigned int r1, unsigned int l3, unsigned int l2, unsigned int l1);
//calls the setIndex function on a whole set of generated vertices to construct the world mesh
void setIndices(std::vector<unsigned int> &indices, std::vector<Vertex> &vertices,
                int worldHeight, int worldWidth);
//sets color of the mesh based on elevation
glm::vec3 setColor(float pos, int biome);
//GENERATES TREES BUT ALSO MAKES ME CRY BECAUSE I DIDNT GET NORMALS TO WORK IN TIME AND ALSO DONT HAVE METHOD TO PLACE THEM 
std::vector<Mesh> genFlora(glm::vec3 startVertexPosition);
//Sets the vertices and generates heights for each triangle in order to build a heightmap for the world
void setWorldVertices(std::vector<Vertex> &vertices,
                      int worldHeight, int worldWidth,
                      int biome, int seed, float frequency,
                      float lacunarity, float gain,
                      int octaves, int scale);
//Generates the world mesh by calling the above helper functions
Mesh genWorld(int worldHeight, int worldWidth,
              int biome, int seed, float frequency,
              float lacunarity, float gain,
              int octaves, int scale);
//Terribly made skybox thats just a cube
Mesh genSkyBox(int worldHeight, int worldWidth);
//Just makes a cube
Mesh genLightSource();
#endif