/*
 *  Shaders
 *
 *  Demonstrate shaders:
 *    Simple shaders
 *    Vertex lighting
 *    Procedural textures
 *    Toon shader
 *    Pixel lighting
 *    Texture lookup
 *    Pixel lighting with texture
 *  If OpenGL 4 is supported:
 *    Pixel lighting using OpenGL 4 style
 *    Draw axes in OpenGL 4 style
 *    Draw text in OpenGL 4 style using geometry shader
 *
 *  Key bindings:
 *  m/M        Cycle through shaders
 *  x/X        Increase/decrease mandelbrot X-value
 *  y/Y        Increase/decrease mandelbrot Y-value
 *  z/Z        Zoom in/out of mandelbrot
 *  o/O        Cycle through objects
 *  p/P        Toggle between orthogonal & perspective projections
 *  s/S        Start/stop light movement
 *  -/+        Decrease/increase light elevation
 *  a          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"
int axes=1;       //  Display axes
int mode=0;       //  Shader mode
int move=1;       //  Move light
int roll=1;       //  Rolling brick texture
int proj=1;       //  Projection type
int obj=1;        //  Object
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int pi=0;         //  Pi texture
int font=0;       //  Font texture
double asp=1;     //  Aspect ratio
double dim=3.0;   //  Size of world
int zh=90;        //  Light azimuth
int suza=0;       //  Object
float Ylight=2;   //  Light elevation
int shader[]  = {0,0,0,0,0,0,0,0,0,0}; //  Shader programs
const char* text[] = {"Fixed Pipeline","Constant Color","Lighting","Brick","Mandelbrot Set","Mandelbrot Hole","Toon Shader","Pixel Lighting","Textures","Pixel Lighting and Texture","OpenGL 4"};
#ifdef GL_VERSION_4_0
#define MODE 11
int shader4[] = {0,0,0}; //  OpenGL 4 shaders
#else
#define MODE 10
#endif
float X=0,Y=0,Z=1; //  Mandelbrot X,Y,Z
//  Light colors
const float Emission[]  = {0.0,0.0,0.0,1.0};
const float Ambient[]   = {0.3,0.3,0.3,1.0};
const float Diffuse[]   = {1.0,1.0,1.0,1.0};
const float Specular[]  = {1.0,1.0,1.0,1.0};
const float Shinyness[] = {16};
//  Transformation matrixes
float ProjectionMatrix[16];
float ViewMatrix[16];
//  Set lighting parameters using uniforms
float Position[4];

/*
 * Set color
 */
void SetColor(float R,float G,float B)
{
   float color[] = {R,G,B,1.0};
   glColor3f(R,G,B);
   glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,color);
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void Cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   //  Front
   SetColor(1,0,0);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,+1);
   glTexCoord2f(0,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,+1);
   glEnd();
   //  Back
   SetColor(0,0,1);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   SetColor(1,1,0);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   SetColor(0,1,0);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   SetColor(0,1,1);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   SetColor(1,0,1);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}

// ***************************************************************
// ******************** Start OpenGL 4 stuff  ********************
// ***************************************************************
#ifdef GL_VERSION_4_0
//
//  4x4 matrix functions
//  Matrices are interpreted as column major order using OpenGL convention
//

//  Identity matrix
static const float I[] = {1,0,0,0 , 0,1,0,0 , 0,0,1,0 , 0,0,0,1};

//
//  Identity 4x4 matrix
//
void mat4identity(float mat[])
{
   memcpy(mat,I,sizeof(I));
}

//
//  Copy 4x4 matrix
//
void mat4copy(float mat[],float m[])
{
   memcpy(mat,m,sizeof(I));
}

//
//  Right multiply 4x4 matrix
//
void mat4multMatrix(float mat[],float m[])
{
   //  res = mat*m
   float res[16];
   for (int i=0;i<4;i++)
      for (int j=0;j<4;j++)
         res[4*i+j] = mat[j]*m[4*i] + mat[4+j]*m[4*i+1] + mat[8+j]*m[4*i+2] + mat[12+j]*m[4*i+3];
   //  Copy matrix back
   memcpy(mat,res,sizeof(res));
}

//
//  Rotate
//
void mat4rotate(float mat[],float th,float x,float y,float z)
{
   //  Normalize axis
   float l = sqrt(x*x+y*y+z*z);
   if (l==0) return;
   x /= l;
   y /= l;
   z /= l;
   //  Calculate sin and cos
   float s = sin(th*M_PI/180);
   float c = cos(th*M_PI/180);
   //  Rotation matrix
   float R[16] =
   {
      (1-c)*x*x+c   , (1-c)*x*y+z*s , (1-c)*z*x-y*s , 0 ,
      (1-c)*x*y-z*s , (1-c)*y*y+c   , (1-c)*y*z+x*s , 0 ,
      (1-c)*z*x+y*s , (1-c)*y*z-x*s , (1-c)*z*z+c   , 0 ,
            0       ,       0       ,       0       , 1 ,
   };
   //  Multiply
   mat4multMatrix(mat,R);
}

//
//  Translate
//
void mat4translate(float mat[],float dx,float dy,float dz)
{
   //  Scale matrix
   float T[16];
   mat4identity(T);
   T[12] = dx;
   T[13] = dy;
   T[14] = dz;
   //  Multiply
   mat4multMatrix(mat,T);
}

//
//  Scale
//
void mat4scale(float mat[],float Sx,float Sy,float Sz)
{
   //  Scale matrix
   float S[16];
   memset(S,0,sizeof(S));
   S[0]  = Sx;
   S[5]  = Sy;
   S[10] = Sz;
   S[15] = 1;
   //  Multiply
   mat4multMatrix(mat,S);
}

//
//  Orthogonal projection matrix
//
void mat4ortho(float mat[],float left,float right,float bottom,float top,float zNear,float zFar)
{
   //  Projection matrix
   float P[16];
   memset(P,0,sizeof(P));
   P[0] = 2/(right-left);
   P[5] = 2/(top-bottom);
   P[10] = -2/(zFar-zNear);
   P[12] = -(right+left)/(right-left);
   P[13] = -(top+bottom)/(top-bottom);
   P[14] = -(zFar+zNear)/(zFar-zNear);
   P[15] = 1;
   //  Multiply
   mat4multMatrix(mat,P);
}

//
//  Perspective projection matrix
//
void mat4perspective(float mat[],float fovy,float asp,float zNear,float zFar)
{
   //  Cotangent
   float s = sin(fovy/2*M_PI/180);
   float c = cos(fovy/2*M_PI/180);
   if (s==0) return;
   float cot = c/s;
   //  Matrix
   float P[16];
   memset(P,0,sizeof(P));
   P[0]  = cot/asp;
   P[5]  = cot;
   P[10] = -(zFar+zNear)/(zFar-zNear);
   P[11] = -1;
   P[14] = -2*zNear*zFar/(zFar-zNear);
   //  Multiply
   mat4multMatrix(mat,P);
}

//
//  Normalize vector
//
static int normalize(float* x,float* y,float* z)
{
   float l = sqrt((*x)*(*x)+(*y)*(*y)+(*z)*(*z));
   if (l==0) return -1;
   *x /= l;
   *y /= l;
   *z /= l;
   return 0;
}

//
//  Set eye position
//
void mat4lookAt(float mat[16] , float Ex,float Ey,float Ez , float Cx,float Cy,float Cz , float Ux,float Uy,float Uz)
{
   //  Forward = C-E
   float Fx = Cx-Ex;
   float Fy = Cy-Ey;
   float Fz = Cz-Ez;
   if (normalize(&Fx,&Fy,&Fz)) return;
   // Side = Forward x Up
   float Sx = Fy*Uz-Uy*Fz;
   float Sy = Fz*Ux-Uz*Fx;
   float Sz = Fx*Uy-Ux*Fy;
   if (normalize(&Sx,&Sy,&Sz)) return;
   //  Recalculate Up = Side x Forward
   Ux = Sy*Fz-Fy*Sz;
   Uy = Sz*Fx-Fz*Sx;
   Uz = Sx*Fy-Fx*Sy;
   //  Rotation (inverse read transposed)
   float R[16] =
   {
    Sx, Ux, -Fx, 0,
    Sy, Uy, -Fy, 0,
    Sz, Uz, -Fz, 0,
    0,  0,    0, 1,
   };
   mat4multMatrix(mat,R);
   //  Set eye at the origin
   mat4translate(mat,-Ex,-Ey,-Ez);
}

//
// Compute inverse of a general 3d transformation matrix.
//    Adapted from graphics gems II.
// 
void mat4normalMatrix(float mat[16],float inv[9])
{
   // Calculate the determinant of upper left 3x3 submatrix
   float det = mat[0]*mat[5]*mat[10]
              +mat[1]*mat[6]*mat[8]
              +mat[2]*mat[4]*mat[9]
              -mat[2]*mat[5]*mat[8]
              -mat[1]*mat[4]*mat[10]
              -mat[0]*mat[6]*mat[9];
   if (det*det<1e-25) return;
   //  Compute inverse using Cramer's rule
   inv[0] =  (mat[5]*mat[10]-mat[6]*mat[9])/det;
   inv[1] = -(mat[4]*mat[10]-mat[6]*mat[8])/det;
   inv[2] =  (mat[4]*mat[ 9]-mat[5]*mat[8])/det;
   inv[3] = -(mat[1]*mat[10]-mat[2]*mat[9])/det;
   inv[4] =  (mat[0]*mat[10]-mat[2]*mat[8])/det;
   inv[5] = -(mat[0]*mat[ 9]-mat[1]*mat[8])/det;
   inv[6] =  (mat[1]*mat[ 6]-mat[2]*mat[5])/det;
   inv[7] = -(mat[0]*mat[ 6]-mat[2]*mat[4])/det;
   inv[8] =  (mat[0]*mat[ 5]-mat[1]*mat[4])/det;
}

//  Cube vertex, normal, color and texture data
const float cube_data[] =
{
//  X  Y  Z  W   Nx Ny Nz    R G B A   s t
   //  Front
   +1,+1,+1,+1,   0, 0,+1,   1,0,0,1,  1,1,
   -1,+1,+1,+1,   0, 0,+1,   1,0,0,1,  0,1,
   +1,-1,+1,+1,   0, 0,+1,   1,0,0,1,  1,0,
   -1,+1,+1,+1,   0, 0,+1,   1,0,0,1,  0,1,
   +1,-1,+1,+1,   0, 0,+1,   1,0,0,1,  1,0,
   -1,-1,+1,+1,   0, 0,+1,   1,0,0,1,  0,0,
   //  Back                        
   -1,-1,-1,+1,   0, 0,-1,   0,0,1,1,  1,0,
   +1,-1,-1,+1,   0, 0,-1,   0,0,1,1,  0,0,
   -1,+1,-1,+1,   0, 0,-1,   0,0,1,1,  1,1,
   +1,-1,-1,+1,   0, 0,-1,   0,0,1,1,  0,0,
   -1,+1,-1,+1,   0, 0,-1,   0,0,1,1,  1,1,
   +1,+1,-1,+1,   0, 0,-1,   0,0,1,1,  0,1,
   //  Right                       
   +1,+1,+1,+1,  +1, 0, 0,   1,1,0,1,  0,1,
   +1,-1,+1,+1,  +1, 0, 0,   1,1,0,1,  0,0,
   +1,+1,-1,+1,  +1, 0, 0,   1,1,0,1,  1,1,
   +1,-1,+1,+1,  +1, 0, 0,   1,1,0,1,  0,0,
   +1,+1,-1,+1,  +1, 0, 0,   1,1,0,1,  1,1,
   +1,-1,-1,+1,  +1, 0, 0,   1,1,0,1,  1,0,
   //  Left                        
   -1,+1,+1,+1,  -1, 0, 0,   0,1,0,1,  1,1,
   -1,+1,-1,+1,  -1, 0, 0,   0,1,0,1,  0,1,
   -1,-1,+1,+1,  -1, 0, 0,   0,1,0,1,  1,0,
   -1,+1,-1,+1,  -1, 0, 0,   0,1,0,1,  0,1,
   -1,-1,+1,+1,  -1, 0, 0,   0,1,0,1,  1,0,
   -1,-1,-1,+1,  -1, 0, 0,   0,1,0,1,  0,0,
   //  Top                         
   +1,+1,+1,+1,   0,+1, 0,   0,1,1,1,  1,0,
   +1,+1,-1,+1,   0,+1, 0,   0,1,1,1,  1,1,
   -1,+1,+1,+1,   0,+1, 0,   0,1,1,1,  0,0,
   +1,+1,-1,+1,   0,+1, 0,   0,1,1,1,  1,1,
   -1,+1,+1,+1,   0,+1, 0,   0,1,1,1,  0,0,
   -1,+1,-1,+1,   0,+1, 0,   0,1,1,1,  0,1,
   //  Bottom                      
   -1,-1,-1,+1,   0,-1, 0,   1,0,1,1,  0,0,
   +1,-1,-1,+1,   0,-1, 0,   1,0,1,1,  1,0,
   -1,-1,+1,+1,   0,-1, 0,   1,0,1,1,  0,1,
   +1,-1,-1,+1,   0,-1, 0,   1,0,1,1,  1,0,
   -1,-1,+1,+1,   0,-1, 0,   1,0,1,1,  0,1,
   +1,-1,+1,+1,   0,-1, 0,   1,0,1,1,  1,1,
   };

/*
 *  Draw a cube
 */
static unsigned int cube_vao=0;
static void Cube4(double x,double y,double z,
                  double dx,double dy,double dz,
                  double th)
{
   //  Select shader
   glUseProgram(shader4[0]);

   //  Once initialized, just bind VAO
   if (cube_vao)
      glBindVertexArray(cube_vao);
   //  Initialize VAO and VBO
   else
   {
      //  Create cube VAO to bind attribute arrays
      glGenVertexArrays(1,&cube_vao);
      glBindVertexArray(cube_vao);

      //  Get buffer name
      unsigned int vbo=0;
      glGenBuffers(1,&vbo);
      //  Bind VBO
      glBindBuffer(GL_ARRAY_BUFFER,vbo);
      //  Copy cube data to VBO
      glBufferData(GL_ARRAY_BUFFER,sizeof(cube_data),cube_data,GL_STATIC_DRAW);

      //  Bind arrays
      //  Vertex
      int loc = glGetAttribLocation(shader4[0],"Vertex");
      glVertexAttribPointer(loc,4,GL_FLOAT,0,52,(void*) 0);
      glEnableVertexAttribArray(loc);
      //  Normal
      loc = glGetAttribLocation(shader4[0],"Normal");
      glVertexAttribPointer(loc,3,GL_FLOAT,0,52,(void*)16);
      glEnableVertexAttribArray(loc);
      //  Color
      loc  = glGetAttribLocation(shader4[0],"Color");
      glVertexAttribPointer(loc,4,GL_FLOAT,0,52,(void*)28);
      glEnableVertexAttribArray(loc);
      //  Texture
      loc  = glGetAttribLocation(shader4[0],"Texture");
      glVertexAttribPointer(loc,2,GL_FLOAT,0,52,(void*)44);
      glEnableVertexAttribArray(loc);
   }

   //  Set Projection and View Matrix
   int id = glGetUniformLocation(shader4[0],"ProjectionMatrix");
   glUniformMatrix4fv(id,1,0,ProjectionMatrix);
   id = glGetUniformLocation(shader4[0],"ViewMatrix");
   glUniformMatrix4fv(id,1,0,ViewMatrix);

   //  Create ModelView matrix
   float ModelViewMatrix[16];
   mat4copy(ModelViewMatrix , ViewMatrix);
   mat4translate(ModelViewMatrix , x,y,z);
   mat4rotate(ModelViewMatrix , th,0,1,0);
   mat4scale(ModelViewMatrix , dx,dy,dz);
   id = glGetUniformLocation(shader4[0],"ModelViewMatrix");
   glUniformMatrix4fv(id,1,0,ModelViewMatrix);
   //  Create Normal matrix
   float NormalMatrix[9];
   mat4normalMatrix(ModelViewMatrix , NormalMatrix);
   id = glGetUniformLocation(shader4[0],"NormalMatrix");
   glUniformMatrix3fv(id,1,0,NormalMatrix);

   //  Set lighting parameters using uniforms
   id = glGetUniformLocation(shader4[0],"Position");
   glUniform4fv(id,1,Position);
   id = glGetUniformLocation(shader4[0],"Ambient");
   glUniform4fv(id,1,Ambient);
   id = glGetUniformLocation(shader4[0],"Diffuse");
   glUniform4fv(id,1,Diffuse);
   id = glGetUniformLocation(shader4[0],"Specular");
   glUniform4fv(id,1,Specular);

   //  Set material properties as uniforms
   id = glGetUniformLocation(shader4[0],"Ks");
   glUniform4fv(id,1,Specular);
   id = glGetUniformLocation(shader4[0],"Ke");
   glUniform4fv(id,1,Emission);
   id = glGetUniformLocation(shader4[0],"Shinyness");
   glUniform1f(id,Shinyness[0]);

   //  Bind Pi texture
   glBindTexture(GL_TEXTURE_2D,pi);
   //  Draw Cube
   glDrawArrays(GL_TRIANGLES,0,36);

   //  Release VAO and VBO
   glBindVertexArray(0);
   glBindBuffer(GL_ARRAY_BUFFER,0);
}

/*
 * Draw a character by selecting character from font texture
 *   This uses a geometry shader to billboard the letter
 *   The letter is placed using the ModelView
 */
static unsigned int font_vao=0;
static void Char(float x,float y,float z,float size,unsigned char ch)
{
   //  Font character dimensions
   const float Sw = 9;
   const float Sh = 16;
   //  Font texture dimensions
   const float Sx = 288;
   const float Sy = 128;

   //  Select shader
   glUseProgram(shader4[2]);

   //  Once initialized, just bind VAO
   if (font_vao)
      glBindVertexArray(font_vao);
   //  Initialize VAO and VBO
   else
   {
      //  Texture coordinates
      //  Font characters are 9x16 arranged in 32x8 array
      //  Texture size is 288x128
      float font_data[256*2];
      int k=0;
      for (int t=128-Sh;t>=0;t-=Sh)
         for (int s=0;s<288;s+=Sw)
         {
            font_data[k++] = s/Sx;
            font_data[k++] = t/Sy;
         }

      //  Create font VAO to bind attribute arrays
      glGenVertexArrays(1,&font_vao);
      glBindVertexArray(font_vao);

      //  Get buffer name
      unsigned int vbo=0;
      glGenBuffers(1,&vbo);
      //  Bind VBO
      glBindBuffer(GL_ARRAY_BUFFER,vbo);
      //  Copy cube data to VBO
      glBufferData(GL_ARRAY_BUFFER,sizeof(font_data),font_data,GL_STATIC_DRAW);

      //  Bind texture array
      int loc  = glGetAttribLocation(shader4[2],"Texture");
      glVertexAttribPointer(loc,2,GL_FLOAT,0,8,(void*)0);
      glEnableVertexAttribArray(loc);
   }

   //  Set Projection Matrix
   int id = glGetUniformLocation(shader4[2],"ProjectionMatrix");
   glUniformMatrix4fv(id,1,0,ProjectionMatrix);
   //  Create ModelView matrix
   float ModelViewMatrix[16];
   mat4copy(ModelViewMatrix , ViewMatrix);
   mat4translate(ModelViewMatrix , x,y,z);
   id = glGetUniformLocation(shader4[2],"ModelViewMatrix");
   glUniformMatrix4fv(id,1,0,ModelViewMatrix);
   //  Set font color
   const float White[] = {1,1,1,1};
   id = glGetUniformLocation(shader4[2],"Kd");
   glUniform4fv(id,1,White);
   //  Character size in world coordinates
   id = glGetUniformLocation(shader4[2],"dX");
   glUniform1f(id,Sw*size/Sh);
   id = glGetUniformLocation(shader4[2],"dY");
   glUniform1f(id,size);
   //  Character size in texture coordinates
   id = glGetUniformLocation(shader4[2],"sX");
   glUniform1f(id,Sw/Sx);
   id = glGetUniformLocation(shader4[2],"sY");
   glUniform1f(id,Sh/Sy);

   //  Bind font texture
   glBindTexture(GL_TEXTURE_2D,font);
   //  Draw Character
   glDrawArrays(GL_POINTS,ch,1);

   //  Release VAO and VBO
   glBindVertexArray(0);
   glBindBuffer(GL_ARRAY_BUFFER,0);
}

//  Axes vertex data
const float axes_data[] =
{
   0,0,0,
   2,0,0,
   0,0,0,
   0,2,0,
   0,0,0,
   0,0,2,
};
static unsigned int axes_vao=0;
/*
 * Draw axes
 */
static void Axes()
{
   //  Select shader
   glUseProgram(shader4[1]);

   //  Once initialized, just bind VAO
   if (axes_vao)
      glBindVertexArray(axes_vao);
   //  Initialize VAO and VBO
   else
   {
      //  Create axes VAO to bind attribute arrays
      glGenVertexArrays(1,&axes_vao);
      glBindVertexArray(axes_vao);

      //  Get buffer name
      unsigned int vbo=0;
      glGenBuffers(1,&vbo);
      //  Bind VBO
      glBindBuffer(GL_ARRAY_BUFFER,vbo);
      //  Copy cube data to VBO
      glBufferData(GL_ARRAY_BUFFER,sizeof(axes_data),axes_data,GL_STATIC_DRAW);

      //  Bind arrays (vertex only)
      int loc = glGetAttribLocation(shader4[1],"Vertex");
      glVertexAttribPointer(loc,3,GL_FLOAT,0,12,(void*) 0);
      glEnableVertexAttribArray(loc);
   }

   //  Set Projection Matrix
   int id = glGetUniformLocation(shader4[1],"ProjectionMatrix");
   glUniformMatrix4fv(id,1,0,ProjectionMatrix);
   //  View matrix is the modelview matrix since model matrix is I
   id = glGetUniformLocation(shader4[1],"ModelViewMatrix");
   glUniformMatrix4fv(id,1,0,ViewMatrix);
   //  Set color
   const float White[] = {1,1,1,1};
   id = glGetUniformLocation(shader4[1],"Kd");
   glUniform4fv(id,1,White);

   //  Draw Axes
   glDrawArrays(GL_LINES,0,6);

   //  Release VAO and VBO
   glBindVertexArray(0);
   glBindBuffer(GL_ARRAY_BUFFER,0);

   //  Label axes (changes shader and VAO)
   Char(2,0,0,0.2,'X');
   Char(0,2,0,0.2,'Y');
   Char(0,0,2,0.2,'Z');
}

/*
 * Draw scene in OpenGL4 style
 */
void Scene4()
{
   //  Create Projection matrix
   mat4identity(ProjectionMatrix);
   if (fov)
      mat4perspective(ProjectionMatrix , fov,asp,dim/16,16*dim);
   else
      mat4ortho(ProjectionMatrix , -dim*asp, +dim*asp, -dim, +dim, -dim, +dim);
   //  Create View matrix
   mat4identity(ViewMatrix);
   if (fov)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      mat4lookAt(ViewMatrix , Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   else
   {
      mat4rotate(ViewMatrix , ph,1,0,0);
      mat4rotate(ViewMatrix , th,0,1,0);
   }
   //  Light position
   Position[0] = 4*Cos(zh);
   Position[1] = Ylight;
   Position[2] = 4*Sin(zh);
   Position[3] = 1;

   //  Now draw the scene (just a cube for now)
   //  To do other objects create a VBO and VAO for each object
   Cube4(0,0,0 , 1,1,1 , 0);

   //  Draw axes
   if (axes) Axes();

   //  Revert to fixed pipeline
   glUseProgram(0);
}
#endif
// *************************************************************
// ******************** End OpenGL 4 stuff  ********************
// *************************************************************

/*
 * Draw scene using the OpenGL2 style
 */
void Scene2()
{
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (proj)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

   //  Draw light position as sphere (still no lighting here)
   float Position[]  = {4*Cos(zh),Ylight,4*Sin(zh),1.0};
   glColor3f(1,1,1);
   glPushMatrix();
   glTranslated(Position[0],Position[1],Position[2]);
   glutSolidSphere(0.03,10,10);
   glPopMatrix();
   //  OpenGL should normalize normal vectors
   glEnable(GL_NORMALIZE);
   //  Enable lighting
   glEnable(GL_LIGHTING);
   //  Enable light 0
   glEnable(GL_LIGHT0);
   //  Set ambient, diffuse, specular components and position of light 0
   glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
   glLightfv(GL_LIGHT0,GL_POSITION,Position);
   //  Set materials
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,Shinyness);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,Specular);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Bind Pi texture
   glBindTexture(GL_TEXTURE_2D,pi);

   //
   //  Draw scene
   //
   //  Select shader (0 => no shader)
   glUseProgram(shader[mode]);
   //  For brick shader set "uniform" variables
   if (mode>0)
   {
      float time = roll ? 0.001*glutGet(GLUT_ELAPSED_TIME) : 0;
      int id = glGetUniformLocation(shader[mode],"Xcenter");
      glUniform1f(id,X);
      id = glGetUniformLocation(shader[mode],"Ycenter");
      glUniform1f(id,Y);
      id = glGetUniformLocation(shader[mode],"Zoom");
      glUniform1f(id,Z);
      id = glGetUniformLocation(shader[mode],"time");
      glUniform1f(id,time);
   }

   //  Draw the objects
   SetColor(0,1,1);
   glEnable(GL_TEXTURE_2D);
   if (obj==2)
      glCallList(suza);
   else if (obj==1)
      glutSolidTeapot(1.0);
   else
      Cube(0,0,0,1,1,1,0);
   glDisable(GL_TEXTURE_2D);

   //  Revert to fixed pipeline
   glUseProgram(0);

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
   if (axes)
   {
      const double len=2.0;  //  Length of axes
      glBegin(GL_LINES);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(len,0.0,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,len,0.0);
      glVertex3d(0.0,0.0,0.0);
      glVertex3d(0.0,0.0,len);
      glEnd();
      //  Label axes
      glRasterPos3d(len,0.0,0.0);
      Print("X");
      glRasterPos3d(0.0,len,0.0);
      Print("Y");
      glRasterPos3d(0.0,0.0,len);
      Print("Z");
   }
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

#ifdef GL_VERSION_4_0
   if (mode==10)
      Scene4();
   else
#endif
      Scene2();

   //  Display parameters
   glWindowPos2i(5,5);
   Print("Angle=%d,%d  Dim=%.1f Projection=%s Mode=%s",
     th,ph,dim,proj?"Perpective":"Orthogonal",text[mode]);
   if (mode==4)
   {
      glWindowPos2i(5,25);
      Print("X=%f Y=%f Z=%f Mag=%f",X,Y,Z,1/Z);
   }
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   if (move) zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(proj?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Toggle projection type
   else if (ch == 'p' || ch == 'P')
      proj = 1-proj;
   //  Toggle light movement
   else if (ch == 's' || ch == 'S')
      move = 1-move;
   //  Toggle brick movement
   else if (ch == 'b' || ch == 'B')
      roll = 1-roll;
   //  Toggle objects
   else if (ch == 'o' || ch == 'O')
      obj = (obj+1)%3;
   //  Cycle modes
   else if (ch == 'm')
      mode = (mode+1)%MODE;
   else if (ch == 'M')
      mode = (mode+MODE-1)%MODE;
   //  Light elevation
   else if (ch == '+')
      Ylight += 0.1;
   else if (ch == '-')
      Ylight -= 0.1;
   //  Mandelbrot
   else if (ch == 'x')
      X += 0.01*Z;
   else if (ch == 'X')
      X -= 0.01*Z;
   else if (ch == 'y')
      Y += 0.01*Z;
   else if (ch == 'Y')
      Y -= 0.01*Z;
   else if (ch == 'z')
      Z *= 0.9;
   else if (ch == 'Z')
      Z *= 1.1;
   else if (ch == 'r' || ch=='R')
   {
      X = Y = 0;
      Z = 1;
   }
   //  Reproject
   Project(proj?fov:0,asp,dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, RES*width,RES*height);
   //  Set projection
   Project(proj?fov:0,asp,dim);
}

/*
 *  Read text file
 */
char* ReadText(char *file)
{
   char* buffer;
   //  Open file
   FILE* f = fopen(file,"rt");
   if (!f) Fatal("Cannot open text file %s\n",file);
   //  Seek to end to determine size, then rewind
   fseek(f,0,SEEK_END);
   int n = ftell(f);
   rewind(f);
   //  Allocate memory for the whole file
   buffer = (char*)malloc(n+1);
   if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
   //  Snarf the file
   if (fread(buffer,n,1,f)!=1) Fatal("Cannot read %d bytes for text file %s\n",n,file);
   buffer[n] = 0;
   //  Close and return
   fclose(f);
   return buffer;
}

/*
 *  Print Shader Log
 */
void PrintShaderLog(int obj,char* file)
{
   int len=0;
   glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n",len);
      glGetShaderInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s:\n%s\n",file,buffer);
      free(buffer);
   }
   glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
   if (!len) Fatal("Error compiling %s\n",file);
}

/*
 *  Print Program Log
 */
void PrintProgramLog(int obj)
{
   int len=0;
   glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n",len);
      glGetProgramInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s\n",buffer);
   }
   glGetProgramiv(obj,GL_LINK_STATUS,&len);
   if (!len) Fatal("Error linking program\n");
}

/*
 *  Create Shader
 */
int CreateShader(GLenum type,char* file)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   char* source = ReadText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   free(source);
   //  Compile the shader
   fprintf(stderr,"Compile %s\n",file);
   glCompileShader(shader);
   //  Check for errors
   PrintShaderLog(shader,file);
   //  Return name
   return shader;
}

/*
 *  Create Shader Program
 */
int CreateShaderProg(char* VertFile,char* FragFile)
{
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   int vert = CreateShader(GL_VERTEX_SHADER,VertFile);
   //  Create and compile fragment shader
   int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
   //  Attach vertex shader
   glAttachShader(prog,vert);
   //  Attach fragment shader
   glAttachShader(prog,frag);
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}

/*
 *  Create Shader Program
 */
#ifdef GL_VERSION_4_0
int CreateShaderGeom(char* VertFile,char* GeomFile,char* FragFile)
{
   //  Create program
   int prog = glCreateProgram();
   //  Compile shaders
   int vert = CreateShader(GL_VERTEX_SHADER  ,VertFile);
   int geom = CreateShader(GL_GEOMETRY_SHADER,GeomFile);
   int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
   //  Attach shaders
   glAttachShader(prog,vert);
   glAttachShader(prog,geom);
   glAttachShader(prog,frag);
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}
#endif

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Shaders");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Load object
   suza = LoadOBJ("suzanne.obj");
   //  Load textures
   pi   = LoadTexBMP("pi.bmp");
   font = LoadTexBMP("font.bmp");
   //  Switch font to nearest
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
   //  Create Shader Programs
   shader[1] = CreateShaderProg("simple.vert","simple.frag");
   shader[2] = CreateShaderProg("phong.vert","phong.frag");
   shader[3] = CreateShaderProg("model.vert","brick.frag");
   shader[4] = CreateShaderProg("model.vert","mandel.frag");
   shader[5] = CreateShaderProg("model.vert","manhole.frag");
   shader[6] = CreateShaderProg("toon.vert","toon.frag");
   shader[7] = CreateShaderProg("pixlight.vert","pixlight.frag");
   shader[8] = CreateShaderProg("texture.vert","texture.frag");
   shader[9] = CreateShaderProg("pixtex.vert","pixtex.frag");
#ifdef GL_VERSION_4_0
   shader4[0] = CreateShaderProg("gl4pix.vert","gl4pix.frag");
   shader4[1] = CreateShaderProg("gl4fix.vert","gl4fix.frag");
   shader4[2] = CreateShaderGeom("gl4tex.vert","gl4tex.geom","gl4tex.frag");
#endif
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
