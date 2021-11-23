#include "CSCIx229.h"
#include "compileshaders.h"
#include "camera.h"

// specifying shader file names
const char *vertexShader = "./Shaders/shader.vs";
const char *fragmentShader = "./Shaders/shader.fs";
unsigned int vao;
unsigned int vbo;
unsigned int ibo;
GLuint shader;
//imported noise library WOOOOOOOOO
float *generateNoise()
{
    // Create and configure FastNoise object
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);

    // Gather noise data
    float noiseData[10];
    int index = 0;

    for (int y = 0; y < 128; y++)
    {
        for (int x = 0; x < 128; x++)
        {
            noiseData[index++] = noise.GetNoise((float)x, (float)y);
        }
    }

    // Do something with this data...

    // Free data later
    return noiseData;
}

void mesh(int height, int width)
{
    //generate mesh data
    //stride is determined by the width of the mesh
    //every WIDTH in array is a NEW ROW
    //BUILDING FROM BOTTOM ROW UPWARDS!
    std::vector<float> coord;
    int row = 0;
    while (row != height)
    {
        for (float i = 0.0f; i < width; i++)
        {
            coord.push_back(i);
            //ELEVATION IS TEMPORARILY 0 SO AS TO CREATE FLAT MESH
            coord.push_back(0);
            coord.push_back(row);
        }
        row++;
    }
    
    for (int i = 0; i < coord.size(); i++)
    {
        std::cout << coord[i] << std::endl;
    }

    float vertices[] =
        {
            -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, //0
            0.5f, -0.5f, 1.0f, 0.5f, 1.0f,  //1
            0.5f, 0.5f, 1.0f, 1.0f, 1.0f,   //2
            -0.5f, 0.5f, 0.5f, 1.0f, 1.0f   //3
        };
    unsigned int indices[] =
        {
            0, 1, 2, //triangle 1
            2, 3, 0  //triangle 2
        };
    //VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 5 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //IBO
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}
/* VERTEX LAYER
    - This is an abstract structure, We first generate a flat mesh using triangles with y-values = 0
*/
void vertexLayer()
{
    //CODE HERE

    //generate height map

    //render the mesh
}
/* BIOME LAYER
    - Groupings of vertices will be designated as regions, different
    regions will have different lists of flora/densities of flora as well as color palettes/elevations 
*/
void biomeLayer()
{
    //CODE HERE
}

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void init()
{
    int height, width;
    height = 2;
    width = 5;
    mesh(height, width);
}

void render()
{
    const static float black[] = {0.0f, 0.0f, 0.0f, 0.0f};
    glClearBufferfv(GL_COLOR, 0, black);
    glBindVertexArray(vao);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int main(void)
{
    GLFWwindow *window;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glewInit();
    glfwSwapInterval(1);
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    //  Set callback for keyboard input
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
    init();
    while (!glfwWindowShouldClose(window))
    {
        shader = CompileShaders(vertexShader, fragmentShader);
        camera.Matrix(45.0f, 0.1f, 100.0f, shader, "camMatrix");
        camera.Inputs(window);
        render();
        glfwSwapBuffers(window);
        glUseProgram(0);
        glfwPollEvents();
    }
}