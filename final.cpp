#include "CSCIx229.h"
#include "compileshaders.h"
// specifying shader file names
const char *vertexShader = "shader.vs";
const char *fragmentShader = "shader.fs";
int th = 0;     //  Azimuth of view angle
int ph = 0;     //  Elevation of view angle
int fov = 55;   //  Field of view (for perspective)
double asp = 1; //  Aspect ratio
double dim = 3; //  Size of world
int mode = 1;

struct Vector3f
{
    float x;
    float y;
    float z;

    Vector3f() {}

    Vector3f(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    Vector3f(const float *pFloat)
    {
        x = pFloat[0];
        y = pFloat[0];
        z = pFloat[0];
    }

    Vector3f(float f)
    {
        x = y = z = f;
    }

    Vector3f &operator+=(const Vector3f &r)
    {
        x += r.x;
        y += r.y;
        z += r.z;

        return *this;
    }

    Vector3f &operator-=(const Vector3f &r)
    {
        x -= r.x;
        y -= r.y;
        z -= r.z;

        return *this;
    }

    Vector3f &operator*=(float f)
    {
        x *= f;
        y *= f;
        z *= f;

        return *this;
    }

    operator const float *() const
    {
        return &(x);
    }

    Vector3f Cross(const Vector3f &v) const;

    Vector3f &Normalize();

    void Rotate(float Angle, const Vector3f &Axis);

    void Print() const
    {
        printf("(%.02f, %.02f, %.02f)", x, y, z);
    }
};

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_M)
        std::cout << "M KEY PRESSED\n";
    Project(fov, asp, dim, th, ph, mode);

}


unsigned int VBO;

static void RenderSceneCB()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    perspectiveMode(ph, th, dim, mode);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}

static void CreateVertexBuffer()
{
    const float Vertices[] = {
                             -.5f, -.5f, 0.0f,
                              .5f, -.5f, 0.0f,
                              0.0f, .5f, 0.0f };
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

int main(void)
{
    GLFWwindow *window;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glewInit();


    glfwSetKeyCallback(window, key_callback);    
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity
    // top
    CreateVertexBuffer();
    CompileShaders(vertexShader, fragmentShader);
    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        Project(fov, asp, dim, th, ph, mode);
        RenderSceneCB();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}