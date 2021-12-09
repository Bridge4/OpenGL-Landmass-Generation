#include "CSCIx229.h"
#include "compileshaders.h"
#include "camera.h"
#include "mesh.h"
// specifying shader file names
const char *vertexShader = "./Shaders/shader.vs";
const char *fragmentShader = "./Shaders/shader.fs";
const char *lightV = "./Shaders/light.vs";
const char *lightF = "./Shaders/light.fs";
int worldHeight = 1000;
int worldWidth = 1000;
int biome = 1;
int scale = 150;
int seed = 1337;
float lightX = worldWidth;
float lightY = 20.0f;
float lightZ = worldHeight;
float frequency = 0.004f;
float lacunarity = 3.0f;
float gain = 0.3f;
int octaves = 16;
bool changeTerrain = false;
bool changeSeed = false;
bool changeBiome = false;
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

int main(void)
{
    GLFWwindow *window;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
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
    glEnable(GL_CULL_FACE);
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    //CAMERA
    Camera camera(width, height, glm::vec3(worldWidth / 2, 40.0f, worldHeight));
    //SKYBOX GENERATION
    Mesh skyBox = genSkyBox(worldHeight, worldWidth);
    //WORLD GENERATION
    Mesh world = genWorld(worldHeight, worldWidth, biome, seed,
                          frequency, lacunarity, gain, octaves, scale);
    //Ok so I got L-System fractal tree generation to work but the surface normals are all wrong and I don't have the willpower
    //to fix them anymore.
    std::vector<std::vector<Mesh> > flora;

    flora.push_back(genFlora(glm::vec3(0,0,0)));

    //LIGHT SOURCE GENERATION
    Mesh lightSource = genLightSource();

    //SHADER COMPILATION
    shader = CompileShaders(vertexShader, fragmentShader);
    light = CompileShaders(lightV, lightF);

    //Setting uniform varialble locations and making it more readable
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
        
        world.Draw();
        skyBox.Draw();
        glUseProgram(light);
        lightSource.Draw();
        camera.Inputs(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glUseProgram(0);
}