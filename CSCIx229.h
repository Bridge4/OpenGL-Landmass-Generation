#ifndef CSCIx229
#define CSCIx229

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <string.h>
#include <iostream>
#include <map>
#include <tuple>
#include <vector>
#include "./FastNoiseLite/Cpp/FastNoiseLite.h"
#include "./glm/glm/vec3.hpp"// glm::vec3
#include "./glm/glm/vec4.hpp" // glm::vec4
#include "./glm/glm/mat4x4.hpp" // glm::mat4
#include "./glm/glm/ext/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale
#include "./glm/glm/ext/matrix_clip_space.hpp" // glm::perspective
#include "./glm/glm/ext/scalar_constants.hpp" // glm::pi
#include"glm/glm/gtc/type_ptr.hpp"
#include"glm/glm/gtx/rotate_vector.hpp"
#include"glm/glm/gtx/vector_angle.hpp"

// GLEW _MUST_ be included first
#ifdef USEGLEW
#include <GL/glew.h>
#endif
//  Get all GL prototypes
#define GL_GLEXT_PROTOTYPES
//  Select SDL, SDL2, GLFW or GLUT
#if defined(SDL2)
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#elif defined(SDL)
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#elif defined(__APPLE__)
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#else
#include <GLFW/glfw3.h>
#endif
//  Make sure GLU and GL are included
#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <OpenGL/gl3.h>
// Tell Xcode IDE to not gripe about OpenGL deprecation
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else

#endif
#define Cos(th) cos(3.14159265/180*(th))
#define Sin(th) sin(3.14159265/180*(th))

#ifdef __cplusplus
extern "C" {
#endif
void Project(double fov,double asp,double dim);
void perspectiveMode(int ph, int th, double dim, int mode);
#ifdef __cplusplus
}
#endif

#endif
