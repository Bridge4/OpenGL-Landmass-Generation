#include "CSCIx229.h"

//following functions were sourced on github: https://github.com/emeiri/ogldev/blob/master/tutorial04/tutorial04.cpp
bool ReadFile(const char* pFileName, std::string& outFile);
void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
GLuint CompileShaders(const char* vertexShader, const char* fragmentShader);
