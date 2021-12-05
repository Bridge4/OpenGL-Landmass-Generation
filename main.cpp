#include "CSCIx229.h"
#include "compileshaders.h"
#include "camera.h"
#include "mesh.h"
// specifying shader file names
const char *vertexShader = "./Shaders/shader.vs";
const char *fragmentShader = "./Shaders/shader.fs";
const char *lightV = "./Shaders/light.vs";
const char *lightF = "./Shaders/light.fs";
unsigned int vao;
unsigned int vbo;
unsigned int ibo;
int worldHeight = 1000;
int worldWidth = 1000;
int normCountLimit = worldHeight * worldWidth;
int normCount = 0;
bool changeBiome = false;
int biome = 1;
int scale = 150;
bool changeSeed = false;
int seed = 1337;
float lightX = worldWidth;
float lightY = 20.0f;
float lightZ = worldHeight;
float frequency = 0.004f;
float lacunarity = 3.0f;
float gain = 0.3f;
int octaves = 16;
bool changeTerrain = false;
int shadeType = 0;
GLuint shader, light;

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        changeBiome = true;
        if (biome == 2)
            biome = 1;
        else
            biome++;
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        changeSeed = true;
        seed = rand() % 2000;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        lightX--;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        lightX++;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        lightZ++;
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        lightZ--;
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            lightY--;
        lightY++;
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        changeTerrain = true;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            frequency -= 0.001;
        else
            frequency += 0.001;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        changeTerrain = true;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            lacunarity -= 0.01;
        else
            lacunarity += 0.01;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        changeTerrain = true;

        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            gain -= 0.01;
        else
            gain += 0.01;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        changeTerrain = true;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            octaves--;
        else
            octaves++;
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        changeTerrain = true;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            scale -= 10;
        scale += 10;
    }
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

void setIndices(std::vector<unsigned int> &indices, std::vector<Vertex> &vertices)
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
    for (int i = 0; i < vertices.size(); i += 1)
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

void setWorldVertices(std::vector<Vertex> &vertices)
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

Mesh genWorld(int worldHeight, int worldWidth, int biome, int seed,
              float frequency, float lacunarity, float gain, int octaves, int scale)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    setWorldVertices(vertices);
    setIndices(indices, vertices);
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
    for (int i = 0; i < locations.size(); i++)
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
    for (int i = 0; i < locations.size(); i++)
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

Mesh cube(glm::vec3 startVertex = glm::vec3(0, 0, 0), float scale = 1, bool leaf = false)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> locations;
    glm::vec3 trunkColor = glm::vec3(0.431, 0.388, 0.239);
    glm::vec3 leafColor = glm::vec3(0.208, 0.345, 0.204);

    glm::vec3 topLeftF = startVertex + glm::vec3(0, scale, 0);
    glm::vec3 bottomRightF = startVertex + glm::vec3(scale, 0, 0);
    glm::vec3 topRightF = startVertex + glm::vec3(scale, scale, 0);
    glm::vec3 topLeftB = startVertex + glm::vec3(0, scale, scale);
    glm::vec3 bottomRightB = startVertex + glm::vec3(scale, 0, scale);
    glm::vec3 topRightB = startVertex + glm::vec3(scale, scale, scale);
    glm::vec3 bottomLeftB = startVertex + glm::vec3(0, 0, scale);

    locations.push_back(startVertex);
    locations.push_back(bottomLeftB);
    locations.push_back(bottomRightB);
    locations.push_back(bottomRightF);
    locations.push_back(topLeftF);
    locations.push_back(topLeftB);
    locations.push_back(topRightB);
    locations.push_back(topRightF);

    for (int i = 0; i < locations.size(); i++)
    {
        Vertex vertex;
        vertex.Position = locations[i];
        if (leaf)
            vertex.Color = leafColor;
        else
            vertex.Color = trunkColor;
        vertices.push_back(vertex);
    }
    unsigned int cubeIndices[] =
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
        indices.push_back(cubeIndices[i]);
    return Mesh(vertices, indices);
}

/*
Mesh genTrunk(float x, float y, float z, float scale)
{

}
*/

std::vector<Mesh> genFlora(float startVertexPosition)
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
        for (int j = 0; j < sentence.size(); j++)
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
    startVertex.Position = glm::vec3(0, 0, 0);
    bool push = false;
    for (int i = 0; i < sentence.size(); i++)
    {
        Vertex nextVertex;
        if (sentence[i] == 'F')
        {
            nextVertex.Position = startVertex.Position + glm::vec3(0, 1, 0);
            glm::vec3 startPos = startVertex.Position;
            flora.push_back(cube(startPos));
            startVertex = nextVertex;
        }
        else if (sentence[i] == '+')
        {
            nextVertex.Position = startVertex.Position + glm::vec3(1, 0, 0);
            glm::vec3 startPos = startVertex.Position;
            flora.push_back(cube(startPos));
            startVertex = nextVertex;
        }
        else if (sentence[i] == '-')
        {
            nextVertex.Position = startVertex.Position + glm::vec3(1, 0, 0);
            glm::vec3 startPos = startVertex.Position;
            flora.push_back(cube(startPos));
            startVertex = nextVertex;
        }
        //store location of leaf
        else if (sentence[i] == 'X')
        {
            nextVertex.Position = startVertex.Position + glm::vec3(0.5, 1, 0);
            startVertex = nextVertex;
            glm::vec3 startPos = startVertex.Position;
            flora.push_back(cube(startPos, 1, true));
        }
        else if (sentence[i] == '[')
        {
            Vertex trueStart = startVertex;
            while (sentence[i] != ']')
            {
                if (sentence[i] == 'F')
                {

                    nextVertex.Position = startVertex.Position + glm::vec3(0, 1, 0);
                    glm::vec3 startPos = startVertex.Position;
                    flora.push_back(cube(startPos));
                    startVertex = nextVertex;
                }
                else if (sentence[i] == '+')
                {
                    nextVertex.Position = startVertex.Position + glm::vec3(1, 0, 0);
                    glm::vec3 startPos = startVertex.Position;
                    flora.push_back(cube(startPos));
                    startVertex = nextVertex;
                }
                else if (sentence[i] == '-')
                {
                    nextVertex.Position = startVertex.Position + glm::vec3(-1, 0, 0);
                    glm::vec3 startPos = startVertex.Position;
                    flora.push_back(cube(startPos));
                    startVertex = nextVertex;
                }
                //LEAF
                else if (sentence[i] == 'X')
                {
                    nextVertex.Position = startVertex.Position + glm::vec3(0.5, 1, 0);
                    startVertex = nextVertex;
                    glm::vec3 startPos = startVertex.Position;
                    flora.push_back(cube(startPos, 1, true));
                }
                i++;
            }
            startVertex = trueStart;
        }
    }
    return flora;
}

int main(void)
{
    GLFWwindow *window;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_DEPTH_BITS, GL_TRUE);
    window = glfwCreateWindow(1920, 1080, "Sayed Abdulmohsen Alhashemi", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    glewInit();
    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    //CAMERA
    //Camera camera(width, height, glm::vec3(worldWidth / 2, 40.0f, worldHeight));
    Camera camera(width, height, glm::vec3(0, 0, 0));
    //SKYBOX GENERATION
    Mesh skyBox = genSkyBox(worldHeight, worldWidth);

    //WORLD GENERATION
    Mesh world = genWorld(worldHeight, worldWidth, biome, seed,
                          frequency, lacunarity, gain, octaves, scale);

    //LIGHT SOURCE GENERATION
    Mesh lightSource = genLightSource();

    //SHADER COMPILATION
    shader = CompileShaders(vertexShader, fragmentShader);
    light = CompileShaders(lightV, lightF);
    std::vector<Mesh> flora = genFlora(0);
    GLint shaderModelMatrix = glGetUniformLocation(shader, "model");
    GLint shaderColorMatrix = glGetUniformLocation(shader, "lightColor");
    GLint shaderCamMatrix = glGetUniformLocation(shader, "camPos");
    GLint lightModelMatrix = glGetUniformLocation(light, "model");
    GLint lightColorMatrix = glGetUniformLocation(light, "lightColor");

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //LIGHTING
        glUseProgram(light);
        glm::vec4 lightColor = glm::vec4(1, 1, 1, 1.0f);
        glm::vec3 lightPos = glm::vec3(lightX, lightY, lightZ);
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);
        //passing light model matrix to light shader
        glUniformMatrix4fv(lightModelMatrix, 1, GL_FALSE, glm::value_ptr(lightModel));
        //passing light color to light shader
        glUniform4f(lightColorMatrix, lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        // Export the camMatrix to the Vertex Shader of the light cube
        camera.Matrix(light, "camMatrix");

        //RENDERING
        glUseProgram(shader);
        glm::vec3 worldPos = glm::vec3(0, 0, 0);
        glm::mat4 worldModel = glm::mat4(1.0f);
        worldModel = glm::translate(worldModel, worldPos);
        //translating the model
        glUniformMatrix4fv(shaderModelMatrix, 1, GL_FALSE, glm::value_ptr(worldModel));
        //passing light color to the default shader
        glUniform4f(shaderColorMatrix, lightColor.x, lightColor.y, lightColor.z, lightColor.w);

        //CAMERA
        camera.updateMatrix(90.0f, 0.1f, 10000.0f);
        camera.Matrix(shader, "camMatrix");
        // Exports the camera Position to the Fragment Shader for specular lighting
        glUniform3f(shaderCamMatrix, camera.Position.x, camera.Position.y, camera.Position.z);

        //GENERATING NEW LANDSCAPES
        if (changeBiome)
        {
            world = genWorld(worldHeight, worldWidth, biome, seed, frequency, lacunarity, gain, octaves, scale);
            changeBiome = false;
        }
        if (changeSeed)
        {
            world = genWorld(worldHeight, worldWidth, biome, seed, frequency, lacunarity, gain, octaves, scale);
            changeSeed = false;
        }
        if (changeTerrain)
        {
            world = genWorld(worldHeight, worldWidth, biome, seed, frequency, lacunarity, gain, octaves, scale);
            changeTerrain = false;
        }

        //Draw Flora
        for (int i = 0; i < flora.size(); i++)
            flora[i].Draw();
        //world.Draw();
        //skyBox.Draw();
        glUseProgram(light);
        lightSource.Draw();
        camera.Inputs(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glUseProgram(0);
}