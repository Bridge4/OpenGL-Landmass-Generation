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
int worldHeight = 50;
int worldWidth = 50;
int normCountLimit = worldHeight * worldWidth;
int normCount = 0;
int biome = 2;
bool changeBiome = false;
GLuint shader, light;

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        changeBiome = true;
        if(biome == 3)
            biome = 1;
        else
            biome++;
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
        rockColor1 = glm::vec3(0.427,0.31,0.431);
        rockColor2 = glm::vec3(0.612, 0.584, 0.514);
        peakColor = glm::vec3(0.937, 0.851, 0.808);
        waterColor1 = glm::vec3(0.706,0.059,0.729);
        waterColor2 = glm::vec3(0.02, 0.51, 0.792);
        waterColor3 = glm::vec3(0., 0.392, 0.58);
        groundColor = glm::vec3(0.922, 0.788, 0.984);
        break;
    case 3:
        shorelineColor = glm::vec3(0.922, 0.988, 0.984);
        rockColor1 = glm::vec3(0.922, 0.988, 0.984);
        rockColor2 = glm::vec3(0.612, 0.584, 0.514);
        peakColor = glm::vec3(0.937, 0.851, 0.808);
        waterColor1 = glm::vec3(0.0, 0.651, 0.984);
        waterColor2 = glm::vec3(0.02, 0.51, 0.792);
        waterColor3 = glm::vec3(0., 0.392, 0.58);
        groundColor = glm::vec3(0.255, 0.616, 0.471);
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
    else if (pos > -5 && pos <0)
        return waterColor1;
    else if (pos < -5 && pos > -10)
        return waterColor2;
    else if (pos < -10)
        return waterColor3;
    //Ground Color
    else
        return groundColor;
}

//helper function to calculate surface normal for flat shading of triangle/polygon
void setNormal(std::vector<Vertex> vertices,
               unsigned int v1, unsigned int v2, unsigned int v3)
{
    /*
    	Set Vector U to (Triangle.p2 minus Triangle.p1)
	    Set Vector V to (Triangle.p3 minus Triangle.p1)
        Set Normal.x to (multiply U.y by V.z) minus (multiply U.z by V.y)
        Set Normal.y to (multiply U.z by V.x) minus (multiply U.x by V.z)
        Set Normal.z to (multiply U.x by V.y) minus (multiply U.y by V.x)
    */
    //std::cout << "CALCULATING NORMALS..." << std::endl;
    glm::vec3 p1 = vertices[v1].Position;
    glm::vec3 p2 = vertices[v2].Position;
    glm::vec3 p3 = vertices[v3].Position;

    glm::vec3 U = p2 - p1;
    glm::vec3 V = p3 - p1;
    glm::vec3 normal;

    normal.x = (U.x * V.z) - (U.z * V.y);
    normal.y = (U.z * V.x) - (U.x * V.z);
    normal.z = (U.x * V.y) - (U.y * V.x);

    vertices[v1].Normal = normal;
    vertices[v2].Normal = normal;
    vertices[v3].Normal = normal;
}

Mesh Tree(int biome)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> locations;
    //this is not pretty I KNOW
    locations.push_back(glm::vec3(-5, -5, 5));
    locations.push_back(glm::vec3(-5, -5, -5));
    locations.push_back(glm::vec3(5, -5, -5));
    locations.push_back(glm::vec3(5, -5, 5));
    locations.push_back(glm::vec3(-5, 5, 5));
    locations.push_back(glm::vec3(-5, 5, -5));
    locations.push_back(glm::vec3(5, 5, -5));
    locations.push_back(glm::vec3(5, 5, 5));
    unsigned int lightIndices[] =
        {
            0, 1, 2,
            0, 2, 3,

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
    for (int i = 0; i < locations.size(); i++)
    {
        Vertex vertex;
        vertex.Position = locations[i];
        //spring
        if (biome == 1)
        {
            vertex.Color = glm::vec3(0.224, 0.478, 0.216);
        }
        //summer
        else if (biome == 2)
        {
            vertex.Color = glm::vec3(0.224, 0.478, 0.216);
        }
        //autumn
        else if (biome == 3)
        {
            vertex.Color = glm::vec3(0.224, 0.478, 0.216);
        }
        //winter
        else if (biome == 4)
        {
            vertex.Color = glm::vec3(0.224, 0.478, 0.216);
        }
        vertices.push_back(vertex);
    }
    return Mesh(vertices, indices);
}

Mesh genWorld(int worldHeight, int worldWidth, int biome)
{
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    noise.SetFrequency(0.01f);
    noise.SetFractalLacunarity(2.0f);
    noise.SetFractalGain(0.6f);
    noise.SetFractalOctaves(16);
    // this setting
    noise.SetFractalType(FastNoiseLite::FractalType_FBm);
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> v;
    //construct vertex positions for mesh
    float row = 0;
    while (row < worldHeight)
    {
        for (float i = 0.0f; i < worldWidth; i++)
        {
            Vertex vertex;
            float elevation = noise.GetNoise((float)i, (float)row);
            float y = 100 * elevation;
            vertex.Color = setColor(y, biome);
            vertex.Position = glm::vec3(i, y, row);
            vertices.push_back(vertex);
        }
        row++;
    }
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
        indices.push_back(r3);
        indices.push_back(r2);
        indices.push_back(r1);
        indices.push_back(l3);
        indices.push_back(l2);
        indices.push_back(l1);
        //flat shading for low poly look
        setNormal(vertices, r3, r2, r1);
        setNormal(vertices, l3, l2, l1);
        normCount++;
        std::cout << normCount << "/" << vertices.size() << std::endl;
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
        vertex.Color = glm::vec3(0.224, 0.635, 0.682);
        vertices.push_back(vertex);
    }
    unsigned int skyIndices[] =
        {
            0, 1, 2,
            0, 2, 3,

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
    //this is not pretty I KNOW
    locations.push_back(glm::vec3(-10, -10, 10));
    locations.push_back(glm::vec3(-10, -10, -10));
    locations.push_back(glm::vec3(10, -10, -10));
    locations.push_back(glm::vec3(10, -10, 10));
    locations.push_back(glm::vec3(-10, 10, 10));
    locations.push_back(glm::vec3(-10, 10, -10));
    locations.push_back(glm::vec3(10, 10, -10));
    locations.push_back(glm::vec3(10, 10, 10));
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

            0, 7, 3,
            0, 4, 7,

            3, 6, 2,
            3, 7, 6,

            2, 5, 1,
            2, 6, 5,

            1, 4, 0,
            1, 5, 4,

            4, 6, 7,
            4, 5, 6
        };
    for (int i = 0; i < 36; i++)
    {
        indices.push_back(lightIndices[i]);
    }
    return Mesh(vertices, indices);
}


GLFWwindow *startGLFW()
{
    GLFWwindow *window;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_DEPTH_BITS, GL_TRUE);
    window = glfwCreateWindow(1000, 1000, "Terrain Generator", NULL, NULL);

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
    return window;
}

int main(void)
{
    GLFWwindow *window = startGLFW();
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    //CAMERA
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 1.0f));

    //WORLD GENERATION
    Mesh world = genWorld(worldHeight, worldWidth, biome);

    //SKYBOX GENERATION
    Mesh skyBox = genSkyBox(worldHeight, worldWidth);

    //LIGHT SOURCE GENERATION
    Mesh lightSource = genLightSource();

    //SHADER COMPILATION
    shader = CompileShaders(vertexShader, fragmentShader);
    light = CompileShaders(lightV, lightF);

    //LIGHTING
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(worldWidth, 100.0f, worldHeight);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 worldPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 worldModel = glm::mat4(1.0f);
    worldModel = glm::translate(worldModel, worldPos);
    glUseProgram(light);
    glUniformMatrix4fv(glGetUniformLocation(light, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(light, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(worldModel));
    glUniform4f(glGetUniformLocation(shader, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shader, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera.updateMatrix(90.0f, 0.1f, 10000.0f);
        glUseProgram(shader);
        camera.Matrix(shader, "camMatrix");
        if (changeBiome)
        {
            world = genWorld(worldHeight, worldWidth, biome);   
            changeBiome = false;
        }
        world.Draw();
        // Exports the camera Position to the Fragment Shader for specular lighting
        glUniform3f(glGetUniformLocation(shader, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        camera.Inputs(window);
        skyBox.Draw();
        glUseProgram(light);
        // Export the camMatrix to the Vertex Shader of the light cube
        camera.Matrix(light, "camMatrix");
        lightSource.Draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glUseProgram(0);
}