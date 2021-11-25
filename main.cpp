#include "CSCIx229.h"
#include "compileshaders.h"
#include "camera.h"
#include "mesh.h"
// specifying shader file names
const char *vertexShader = "./Shaders/shader.vs";
const char *fragmentShader = "./Shaders/shader.fs";
unsigned int vao;
unsigned int vbo;
unsigned int ibo;
int mHeight = 100;
int mWidth = 100;
GLuint shader;
//imported noise library WOOOOOOOOO
Mesh makeMesh(int mHeight, int mWidth)
{
    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<float> v;
    //construct vertex positions for mesh
    float row = 0;
    while (row < mHeight)
    {
        for (float i = 0.0f; i < mWidth; i++)
        {
            Vertex vertex;
            vertex.Position = glm::vec3(i, row, 100*noise.GetNoise((float)i, (float)row));
            if((int(i) % 2 == 0))
                vertex.Color = glm::vec3(1.0f, 0.0f, 0.0f);
            else    
                vertex.Color = glm::vec3(0.0f, 1.0f, 0.0f);
            vertices.push_back(vertex);
        }
        row++;
    }
    //DRAW ORDER OF TRIANGLES
    //right facing triangle draw order
    unsigned int r1, r2, r3;
    r1 = mWidth;
    r2 = 0;
    r3 = 1;
    //left facing triangle draw order
    unsigned int l1, l2, l3;
    l1 = mWidth+1;
    l2 = mWidth;
    l3 = 1;
    //total drawn in current row
    int inRowRight = 0;
    int inRowLeft = 0;
    //total drawn overall
    int drawnRight = 0;
    int drawnLeft = 0;
    //limit of triangles to draw
    int limit = (mWidth-1)*(mHeight-1);
    for (int i = 0; i < vertices.size(); i += 1)
    {
        if (drawnRight == limit)
            break;
        indices.push_back(r1);
        indices.push_back(r2);
        indices.push_back(r3);
        inRowRight++;
        drawnRight++;
        if (inRowRight == mWidth - 1)
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
    for (int i = 0; i < vertices.size(); i += 1)
    {
        if (drawnLeft == limit)
            break;
        indices.push_back(l1);
        indices.push_back(l2);
        indices.push_back(l3);
        inRowLeft++;
        drawnLeft++;
        if (inRowLeft == mWidth-1)
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
    }

    Mesh mesh(vertices, indices);
    return mesh;
}

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
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
    glfwWindowHint(GLFW_DEPTH_BITS, GL_TRUE);
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
    glEnable(GL_DEPTH_TEST);  
    //generateNoise(mHeight, mWidth);
    //  Set callback for keyboard input
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
    Mesh Mesh = makeMesh(mHeight, mWidth);
    shader = CompileShaders(vertexShader, fragmentShader);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        camera.Matrix(90.0f, 0.1f, 1000.0f, shader, "camMatrix");
        camera.Inputs(window);
        Mesh.Draw();
        glUseProgram(shader);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glUseProgram(0);
}