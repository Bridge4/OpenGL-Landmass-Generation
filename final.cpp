#include "CSCIx229.h"
#include "compileshaders.h"
#include "camera.h"
// specifying shader file names
const char *vertexShader = "shader.vs";
const char *fragmentShader = "shader.fs";
int th = 0;     //  Azimuth of view angle
int ph = 0;     //  Elevation of view angle
int Th = 0;     //  Azimuth of view angle
int Ph = 0;     //  Elevation of view angle
int fov = 55;   //  Field of view (for perspective)
double asp = 1; //  Aspect ratio
double dim = 3; //  Size of world
int mode = 0;
unsigned int vao;
unsigned int vbo;
glm::vec2 rotate;
glm::mat4 projection;
float translate;
GLuint shader;
//imported noise library WOOOOOOOOO
float* generateNoise()
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

/* VERTEX LAYER
    - This is an abstract structure, We first generate a flat mesh using triangles with y-values = 0
*/
void vertexLayer()
{
    //CODE HERE
}
/* BIOME LAYER
    - Groupings of vertices will be designated as regions, different
    regions will have different lists of flora/densities of flora as well as color palettes/elevations 
*/
void biomeLayer()
{
    //CODE HERE
}

void key(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    //  Discard key releases (keeps PRESS and REPEAT)
    if (action == GLFW_RELEASE)
        return;

    //  Check for shift
    int shift = (mods & GLFW_MOD_SHIFT);

    //  Exit on ESC
    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, 1);
    //  Reset view angle
    else if (key == GLFW_KEY_0)
        th = ph = 0;
    //  Switch projection mode
    else if (key == GLFW_KEY_P)
        mode = 1 - mode;
    //  Increase/decrease spot azimuth
    else if (key == GLFW_KEY_LEFT_BRACKET && !shift)
        Ph -= 5;
    else if (key == GLFW_KEY_RIGHT_BRACKET && !shift)
        Ph += 5;
    //  Increase/decrease asimuth
    else if (key == GLFW_KEY_RIGHT)
        rotate.x += 1;
    else if (key == GLFW_KEY_LEFT)
        rotate.x -= 1;
    //  Increase/decrease elevation
    else if (key == GLFW_KEY_UP)
        rotate.y += 1;
    else if (key == GLFW_KEY_DOWN)
        rotate.y -= 1;
    //  PageUp key - increase dim
    else if (key == GLFW_KEY_PAGE_DOWN)
        dim += 0.1;
    //  PageDown key - decrease dim
    else if (key == GLFW_KEY_PAGE_UP && dim > 1)
        dim -= 0.1;
    //  Wrap angles
    Th %= 360;
    Ph %= 360;
    th %= 360;
    ph %= 360;
    //  Update projection
    Project(mode ? fov : 0, asp, dim);
}

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void reshape(GLFWwindow *window, int width, int height)
{
    //  Get framebuffer dimensions (makes Apple work right)
    glfwGetFramebufferSize(window, &width, &height);
    //  Ratio of the width to the height of the window
    asp = (height > 0) ? (double)width / height : 1;
    //  Set the viewport to the entire window
    glViewport(0, 0, width, height);
    //  Set projection
    Project(mode ? fov : 0, asp, dim);
}

void init()
{
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    float vertices[] =
        {
            -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, //top left
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f,  // top right
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,

            0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f
        };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, false, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, false, 5 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void render()
{
    const static float black[] = {0.0f, 0.0f, 0.0f, 0.0f};
    glClearBufferfv(GL_COLOR, 0, black);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
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
    //  Set callback for window resize
    glfwSetWindowSizeCallback(window, reshape);
    //  Set initial window size
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    //  Set callback for keyboard input
    glfwSetKeyCallback(window, key);
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
    init();
    while (!glfwWindowShouldClose(window))
    {
        shader = CompileShaders(vertexShader, fragmentShader);
        camera.Inputs(window);
        camera.Matrix(45.0f, 0.1f, 100.0f, shader, "camMatrix");
        render();
        glfwSwapBuffers(window);
        glUseProgram(0);
        glfwPollEvents();
    }
    
}