#include "CSCIx229.h"

// code for perspectiveGL sourced from here: https://stackoverflow.com/questions/12943164/replacement-for-gluperspective-with-glfrustrum
void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    // fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan(fovY / 360 * pi) * zNear;
    fW = fH * aspect;

    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

void Project(double fov,double asp,double dim)
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (fov)
      gluPerspective(fov,asp,dim/16,16*dim);
   //  Orthogonal transformation
   else
      glOrtho(-asp*dim,asp*dim,-dim,+dim,-dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

void perspectiveMode(int ph, int th, double dim, int mode)
{
    double Ex = -2 * dim * Sin(th) * Cos(ph);
    double Ey = +2 * dim * Sin(ph);
    double Ez = +2 * dim * Cos(th) * Cos(ph);
    //  Set view angle
    if (mode == 1)
    {
        gluLookAt(Ex, Ey, Ez, 0, 0, 0, 0, Cos(ph), 0);
    }
    else if (mode == 2)
    {
        gluLookAt(Ex, Ey, Ez, 0, 0, 0, 0, Cos(ph), 0);
    }
    //  Orthogonal - set world orientation
    else if (mode == 0)
    {
        glRotatef(ph, 1, 0, 0);
        glRotatef(th, 0, 1, 0);
    }
}