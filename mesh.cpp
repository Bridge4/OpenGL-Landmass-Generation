#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
{
    this->vertices = vertices;
    this->indices = indices;
    setupMesh();
}

void Mesh::setupMesh()
{
    //create buffer objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //bind buffers
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    //vertex data
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);
    //index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex colors
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Color));
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Normal));
}

void Mesh::Draw(int primType)
{
    // draw mesh
    glBindVertexArray(VAO);
    if (primType == 1)
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    else if (primType == 2)
        glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
    else
        glDrawElements(GL_POLYGON, indices.size(), GL_UNSIGNED_INT, 0);
    //glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void setNormal(std::vector<Vertex> &vertices,
               unsigned int v1, unsigned int v2, unsigned int v3)
{
    /*
        From khronos website
    	Set Vector U to (Triangle.p2 minus Triangle.p1)
	    Set Vector V to (Triangle.p3 minus Triangle.p1)
        Set Normal.x to (multiply U.y by V.z) minus (multiply U.z by V.y)
        Set Normal.y to (multiply U.z by V.x) minus (multiply U.x by V.z)
        Set Normal.z to (multiply U.x by V.y) minus (multiply U.y by V.x)
    */
    glm::vec3 A = vertices[v1].Position;
    glm::vec3 B = vertices[v2].Position;
    glm::vec3 C = vertices[v3].Position;

    //From my lighting homework

    //  VECTOR FROM A TO B
    float dx0 = A.x - B.x;
    float dy0 = A.y - B.y;
    float dz0 = A.z - B.z;
    //  VECTOR FROM C TO A
    float dx1 = C.x - A.x;
    float dy1 = C.y - A.y;
    float dz1 = C.z - A.z;
    //  Normal
    float Nx = dy0 * dz1 - dy1 * dz0;
    float Ny = dz0 * dx1 - dz1 * dx0;
    float Nz = dx0 * dy1 - dx1 * dy0;

    glm::vec3 normal = glm::vec3(Nx, Ny, Nz);
    vertices[v1].Normal = normal;
    vertices[v2].Normal = normal;
    vertices[v3].Normal = normal;
}

void setIndex(std::vector<unsigned int> &indices, unsigned int r3, unsigned int r2,
              unsigned int r1, unsigned int l3, unsigned int l2, unsigned int l1)
{
    indices.push_back(r3);
    indices.push_back(r2);
    indices.push_back(r1);
    indices.push_back(l3);
    indices.push_back(l2);
    indices.push_back(l1);
}

void setIndices(std::vector<unsigned int> &indices, std::vector<Vertex> &vertices,
                int worldHeight, int worldWidth)
{
    //DRAW ORDER OF TRIANGLES
    //right facing triangle draw order
    unsigned int r1, r2, r3;
    r1 = worldWidth;
    r2 = 0;
    r3 = 1;
    //left facing triangle draw order
    unsigned int l1, l2, l3;
    l1 = worldWidth + 1;
    l2 = worldWidth;
    l3 = 1;
    //total drawn in current row
    int inRowRight = 0;
    int inRowLeft = 0;
    //total drawn overall
    int drawnRight = 0;
    int drawnLeft = 0;
    //limit of triangles to draw
    int limit = (worldWidth - 1) * (worldHeight - 1);

    for (int i = 0; i < (int) (int) vertices.size(); i += 1)
    {
        if (drawnRight == limit && drawnLeft == limit)
            break;
        //Draw order for right-facing and left-facing triangles
        setIndex(indices, r3, r2, r1, l3, l2, l1);
        setNormal(vertices, r3, r2, r1);
        setNormal(vertices, l3, l2, l1);
        inRowLeft++;
        drawnLeft++;
        inRowRight++;
        drawnRight++;
        if (inRowLeft == worldWidth - 1)
        {
            l1 += 2;
            l2 += 2;
            l3 += 2;
            inRowLeft = 0;
        }
        else
        {
            l1++;
            l2++;
            l3++;
        }
        if (inRowRight == worldWidth - 1)
        {
            r1 += 2;
            r2 += 2;
            r3 += 2;
            inRowRight = 0;
        }
        else
        {
            r1++;
            r2++;
            r3++;
        }
    }
}

glm::vec3 setColor(float pos, int biome)
{
    glm::vec3 shorelineColor;
    glm::vec3 rockColor1;
    glm::vec3 rockColor2;
    glm::vec3 peakColor;
    glm::vec3 waterColor1;
    glm::vec3 waterColor2;
    glm::vec3 waterColor3;
    glm::vec3 groundColor;

    switch (biome)
    {
    case 1:
        shorelineColor = glm::vec3(0.922, 0.988, 0.984);
        rockColor1 = glm::vec3(0.922, 0.988, 0.984);
        rockColor2 = glm::vec3(0.612, 0.584, 0.514);
        peakColor = glm::vec3(0.937, 0.851, 0.808);
        waterColor1 = glm::vec3(0.0, 0.651, 0.984);
        waterColor2 = glm::vec3(0.02, 0.51, 0.792);
        waterColor3 = glm::vec3(0., 0.392, 0.58);
        groundColor = glm::vec3(0.255, 0.616, 0.471);
        break;
    case 2:
        shorelineColor = glm::vec3(0.922, 0.988, 0.984);
        rockColor1 = glm::vec3(0.427, 0.31, 0.431);
        rockColor2 = glm::vec3(0.612, 0.584, 0.514);
        peakColor = glm::vec3(0.831, 0.816, 0.839);
        waterColor1 = glm::vec3(0.706, 0.059, 0.729);
        waterColor2 = glm::vec3(0.02, 0.51, 0.792);
        waterColor3 = glm::vec3(0., 0.392, 0.58);
        groundColor = glm::vec3(0.922, 0.788, 0.984);
        break;
    }
    //Shoreline color
    if (pos < 5 && pos > 0)
        return shorelineColor;
    //Rock colors
    else if (pos < 10 && pos > 5)
        return rockColor1;
    else if (pos < 15 && pos > 10)
        return rockColor2;
    //Peak colors
    else if (pos > 20)
        return peakColor;
    //Water Colors
    else if (pos > -5 && pos < 0)
        return waterColor1;
    else if (pos < -5 && pos > -10)
        return waterColor2;
    else if (pos < -10)
        return waterColor3;
    //Ground Color
    else
        return groundColor;
}

//THIS IS WORKING L-SYSTEM CODE THAT I COULDNT GET SURFACE NORMALS TO WORK FOR
//THIS GENERATES TREES PLEASE BELIEVE ME
std::vector<Mesh> genFlora(glm::vec3 startVertexPosition)
{
    //USING L-SYSTEMS TO GENERATE FRACTAL FLORA
    std::vector<Mesh> flora;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    std::string sentence;
    std::string next;
    int limit = 2;
    //starter
    sentence.append("X");
    //rules
    std::string rule1 = "X";
    std::string rule2 = "F+[-F-XF-X][+FF][--X[+X]][++F-X]";
    for (int i = 0; i < limit; i++)
    {
        std::cout << sentence << std::endl;
        for (int j = 0; j < (int) sentence.size(); j++)
        {
            if (sentence[j] == 'F')
            {
                next.append(rule1);
            }
            else if (sentence[j] == 'X')
            {
                next.append(rule2);
            }
        }
        sentence = next;
        next = "";
    }
    //construct shapes based on chars in sentence
    Vertex startVertex;
    startVertex.Position = startVertexPosition;
    for (int i = 0; i < (int) sentence.size(); i++)
    {
        Vertex nextVertex;
        if (sentence[i] == 'F')
        {
            nextVertex.Position = startVertex.Position + glm::vec3(0, 1, 0);
            startVertex = nextVertex;
        }
        else if (sentence[i] == '+')
        {
            nextVertex.Position = startVertex.Position + glm::vec3(1, 0, 0);
            startVertex = nextVertex;
        }
        else if (sentence[i] == '-')
        {
            nextVertex.Position = startVertex.Position + glm::vec3(1, 0, 0);
            startVertex = nextVertex;
        }
        //store location of leaf
        else if (sentence[i] == 'X')
        {
            nextVertex.Position = startVertex.Position + glm::vec3(0.5, 1, 0);
            startVertex = nextVertex;
        }
        else if (sentence[i] == '[')
        {
            Vertex trueStart = startVertex;
            while (sentence[i] != ']')
            {
                if (sentence[i] == 'F')
                {

                    nextVertex.Position = startVertex.Position + glm::vec3(0, 1, 0);
                    startVertex = nextVertex;
                }
                else if (sentence[i] == '+')
                {
                    nextVertex.Position = startVertex.Position + glm::vec3(1, 0, 0);
                    startVertex = nextVertex;
                }
                else if (sentence[i] == '-')
                {
                    nextVertex.Position = startVertex.Position + glm::vec3(-1, 0, 0);
                    startVertex = nextVertex;
                }
                //LEAF
                else if (sentence[i] == 'X')
                {
                    nextVertex.Position = startVertex.Position + glm::vec3(0.5, 1, 0);
                    startVertex = nextVertex;
                }
                i++;
            }
            startVertex = trueStart;
        }
    }
    return flora;
}

void setWorldVertices(std::vector<Vertex> &vertices,
                      int worldHeight, int worldWidth,
                      int biome, int seed, float frequency,
                      float lacunarity, float gain,
                      int octaves, int scale)
{
    FastNoiseLite noise;
    noise.SetSeed(seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(frequency);
    noise.SetFractalLacunarity(lacunarity);
    noise.SetFractalGain(gain);
    noise.SetFractalOctaves(octaves);
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    //construct vertex positions for mesh
    float row = 0;

    while (row < worldHeight)
    {
        for (float i = 0.0f; i < worldWidth; i++)
        {
            Vertex vertex;
            float elevation = noise.GetNoise((float)i, (float)row);
            float y = scale * elevation;
            vertex.Color = setColor(y, biome);
            vertex.Position = glm::vec3(i, y, row);
            vertices.push_back(vertex);
        }
        row++;
    }
}

Mesh genWorld(int worldHeight, int worldWidth,
              int biome, int seed, float frequency,
              float lacunarity, float gain,
              int octaves, int scale)

{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    setWorldVertices(vertices, worldHeight, worldWidth, biome, seed,
                     frequency, lacunarity, gain, octaves, scale);
    setIndices(indices, vertices, worldHeight, worldWidth);
    return Mesh(vertices, indices);
}

Mesh genSkyBox(int worldHeight, int worldWidth)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> locations;
    float skyBoxSize;
    if (worldHeight > worldWidth)
        skyBoxSize = (float)worldHeight * 3;
    else
        skyBoxSize = (float)worldWidth * 3;
    //this is not pretty I KNOW
    locations.push_back(glm::vec3(-skyBoxSize, -skyBoxSize, skyBoxSize));
    locations.push_back(glm::vec3(-skyBoxSize, -skyBoxSize, -skyBoxSize));
    locations.push_back(glm::vec3(skyBoxSize, -skyBoxSize, -skyBoxSize));
    locations.push_back(glm::vec3(skyBoxSize, -skyBoxSize, skyBoxSize));
    locations.push_back(glm::vec3(-skyBoxSize, skyBoxSize, skyBoxSize));
    locations.push_back(glm::vec3(-skyBoxSize, skyBoxSize, -skyBoxSize));
    locations.push_back(glm::vec3(skyBoxSize, skyBoxSize, -skyBoxSize));
    locations.push_back(glm::vec3(skyBoxSize, skyBoxSize, skyBoxSize));
    for (int i = 0; i < (int) locations.size(); i++)
    {
        Vertex vertex;
        vertex.Position = locations[i];
        vertex.Color = glm::vec3(1, 1, 1);
        vertices.push_back(vertex);
    }
    unsigned int skyIndices[] =
        {
            2, 1, 0,
            3, 2, 0,

            0, 7, 3,
            0, 4, 7,

            3, 6, 2,
            3, 7, 6,

            2, 5, 1,
            2, 6, 5,

            1, 4, 0,
            1, 5, 4,

            4, 6, 7,
            4, 5, 6};
    for (int i = 0; i < 36; i++)
    {
        indices.push_back(skyIndices[i]);
    }
    return Mesh(vertices, indices);
}

Mesh genLightSource()
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> locations;

    //front bottom left
    locations.push_back(glm::vec3(-1, -1, 1));
    //back bottom left
    locations.push_back(glm::vec3(-1, -1, -1));
    //back bottom right
    locations.push_back(glm::vec3(1, -1, -1));
    //front bottom right
    locations.push_back(glm::vec3(1, -1, 1));
    //front top left
    locations.push_back(glm::vec3(-1, 1, 1));
    //back top left
    locations.push_back(glm::vec3(-1, 1, -1));
    //back top right
    locations.push_back(glm::vec3(1, 1, -1));
    //front top right
    locations.push_back(glm::vec3(1, 1, 1));
    for (int i = 0; i < (int) locations.size(); i++)
    {
        Vertex vertex;
        vertex.Position = locations[i];
        vertices.push_back(vertex);
    }
    unsigned int lightIndices[] =
        {
            0, 1, 2,
            0, 2, 3,

            3, 7, 0,
            7, 4, 0,

            2, 6, 3,
            6, 7, 3,

            1, 5, 2,
            5, 6, 2,

            0, 4, 1,
            4, 5, 1,

            7, 6, 4,
            6, 5, 4};
    for (int i = 0; i < 36; i++)
    {
        indices.push_back(lightIndices[i]);
    }
    return Mesh(vertices, indices);
}
