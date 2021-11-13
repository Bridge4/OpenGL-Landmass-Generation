#include "CSCIx229.h"
#include "compileshaders.h"
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
int mode = 1;
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
        th += 5;
    else if (key == GLFW_KEY_LEFT)
        th -= 5;
    //  Increase/decrease elevation
    else if (key == GLFW_KEY_UP)
        ph += 5;
    else if (key == GLFW_KEY_DOWN)
        ph -= 5;
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
    Project(mode ? fov : 0, asp, dim, th, ph, mode);
}

static void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

const int Ni=60;
//  Vertex coordinates and colors
const float xyzrgb[] =
{
    0.276, 0.851, 0.447,  0.0,0.0,1.0,
    0.894, 0.000, 0.447,  0.0,0.0,1.0,
    0.000, 0.000, 1.000,  0.0,0.0,1.0,
   -0.724, 0.526, 0.447,  0.0,1.0,0.0,
    0.276, 0.851, 0.447,  0.0,1.0,0.0,
    0.000, 0.000, 1.000,  0.0,1.0,0.0,
   -0.724,-0.526, 0.447,  0.0,1.0,1.0,
   -0.724, 0.526, 0.447,  0.0,1.0,1.0,
    0.000, 0.000, 1.000,  0.0,1.0,1.0,
    0.276,-0.851, 0.447,  1.0,0.0,1.0,
   -0.724,-0.526, 0.447,  1.0,0.0,1.0,
    0.000, 0.000, 1.000,  1.0,0.0,1.0,
    0.894, 0.000, 0.447,  1.0,1.0,0.0,
    0.276,-0.851, 0.447,  1.0,1.0,0.0,
    0.000, 0.000, 1.000,  1.0,1.0,0.0,
    0.000, 0.000,-1.000,  0.0,0.0,1.0,
    0.724, 0.526,-0.447,  0.0,0.0,1.0,
   -0.276, 0.851,-0.447,  0.0,0.0,1.0,
    0.000, 0.000,-1.000,  0.0,1.0,0.0,
   -0.276, 0.851,-0.447,  0.0,1.0,0.0,
   -0.894, 0.000,-0.447,  0.0,1.0,0.0,
    0.000, 0.000,-1.000,  0.0,1.0,1.0,
   -0.894, 0.000,-0.447,  0.0,1.0,1.0,
   -0.276,-0.851,-0.447,  0.0,1.0,1.0,
    0.000, 0.000,-1.000,  1.0,0.0,0.0,
   -0.276,-0.851,-0.447,  1.0,0.0,0.0,
    0.724,-0.526,-0.447,  1.0,0.0,0.0,
    0.000, 0.000,-1.000,  1.0,0.0,1.0,
    0.724,-0.526,-0.447,  1.0,0.0,1.0,
    0.724, 0.526,-0.447,  1.0,0.0,1.0,
    0.894, 0.000, 0.447,  1.0,1.0,0.0,
    0.276, 0.851, 0.447,  1.0,1.0,0.0,
    0.724, 0.526,-0.447,  1.0,1.0,0.0,
    0.276, 0.851, 0.447,  0.0,0.0,1.0,
   -0.724, 0.526, 0.447,  0.0,0.0,1.0,
   -0.276, 0.851,-0.447,  0.0,0.0,1.0,
   -0.724, 0.526, 0.447,  0.0,1.0,0.0,
   -0.724,-0.526, 0.447,  0.0,1.0,0.0,
   -0.894, 0.000,-0.447,  0.0,1.0,0.0,
   -0.724,-0.526, 0.447,  0.0,1.0,1.0,
    0.276,-0.851, 0.447,  0.0,1.0,1.0,
   -0.276,-0.851,-0.447,  0.0,1.0,1.0,
    0.276,-0.851, 0.447,  1.0,0.0,0.0,
    0.894, 0.000, 0.447,  1.0,0.0,0.0,
    0.724,-0.526,-0.447,  1.0,0.0,0.0,
    0.276, 0.851, 0.447,  1.0,0.0,1.0,
   -0.276, 0.851,-0.447,  1.0,0.0,1.0,
    0.724, 0.526,-0.447,  1.0,0.0,1.0,
   -0.724, 0.526, 0.447,  1.0,1.0,0.0,
   -0.894, 0.000,-0.447,  1.0,1.0,0.0,
   -0.276, 0.851,-0.447,  1.0,1.0,0.0,
   -0.724,-0.526, 0.447,  0.0,0.0,1.0,
   -0.276,-0.851,-0.447,  0.0,0.0,1.0,
   -0.894, 0.000,-0.447,  0.0,0.0,1.0,
    0.276,-0.851, 0.447,  0.0,1.0,0.0,
    0.724,-0.526,-0.447,  0.0,1.0,0.0,
   -0.276,-0.851,-0.447,  0.0,1.0,0.0,
    0.894, 0.000, 0.447,  0.0,1.0,1.0,
    0.724, 0.526,-0.447,  0.0,1.0,1.0,
    0.724,-0.526,-0.447,  0.0,1.0,1.0,
};

/*
 *  Draw icosahedron using client side arrays
 *     at (x,y,z)
 *     size  s
 *     rotated th about the x axis
 */
static void icosahedron2(float x,float y,float z,float s,float th)
{
   //  Define vertexes
   glVertexPointer(3,GL_FLOAT,6*sizeof(float),xyzrgb);
   glEnableClientState(GL_VERTEX_ARRAY);
   //  Define colors for each vertex
   //  Draw icosahedron
   glPushMatrix();
   glTranslatef(x,y,z);
   glRotatef(th,1,0,0);
   glScalef(s,s,s);
   glDrawArrays(GL_TRIANGLES,0,Ni);
   glPopMatrix();
   //  Disable vertex array
   glDisableClientState(GL_VERTEX_ARRAY);
   //  Disable color array
   glDisableClientState(GL_COLOR_ARRAY);
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
    Project(mode ? fov : 0, asp, dim, th, ph, mode);
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
    glfwSwapInterval(1);
    glfwSetWindowSizeCallback(window, reshape);
    int width, height;

    glfwGetWindowSize(window, &width, &height);
    reshape(window, width, height);
    glfwSetKeyCallback(window, key);
    
    CompileShaders(vertexShader, fragmentShader);
    
    while (!glfwWindowShouldClose(window))
    {
        icosahedron2(0,0,0, 1, 0);
        glfwSwapBuffers(window);
        glUseProgram(0);
        glfwPollEvents();
    }
}