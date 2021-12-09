#include "compileshaders.h"
/*
    The following functions are all derived/reused directly from the link provided in the header file
    The reason why I opted to reuse the code is because this was a fairly tedious process, I understand EVERY PART of this
    and had implemented it on my own before reusing it. 

    The ReadFile function simply reads the string from the shader file name specified and returns it

    The AddShader function compiles and attaches a shader to the shader program specified, it will do this for the 
    vertex shader and the fragment shader

    The reason I used this is so that creating new shaders is made easier in the long run, it abstracts away all the tedious
    work that comes with shader compilation in openGL so that I can just focus on writing shader code. In this project the 
    shader code was very simple, it just did some basic Blinn Phong Lighting and adjusted the scene relative to the camera for
    camera movement and inputs. It was important to me to implement this regardless of the simple shader code because I didn't know
    how deep I was going to go into shader programming and having the option to expand my scope was nice. 

    The CompileShader function compiles and links the vertex and fragment shaders into a shader program and then returns the 
    GLuint representation of that program so that the user can run the glUseProgram() function on any specified shader program
    they choose to create. This was a change I implemented.
*/
bool ReadFile(const char* pFileName, std::string& outFile){
    std::ifstream f(pFileName);

    bool ret = false;

    if(f.is_open()){
        std::string line;
        while(getline(f, line)){
            outFile.append(line);
            outFile.append("\n");
        }
        f.close();
        ret = true;
    }
    else {
        fprintf(stderr, "ERROR READING FILE\n");
    }
    return ret;
}

void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{   
    //Creating Shader Object
    GLuint ShaderObj = glCreateShader(ShaderType);

    //Error Checking
    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }

    //Storing the shader code in the shader text
    const GLchar* p[1];
    p[0] = pShaderText;
    
    GLint Lengths[1];
    Lengths[0] = (GLint)strlen(pShaderText);

    //Specifying the shader source
    glShaderSource(ShaderObj, 1, p, Lengths);

    //Compiling the chosen shader object
    glCompileShader(ShaderObj);

    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    glAttachShader(ShaderProgram, ShaderObj);
}

GLuint CompileShaders(const char* vertexShader, const char* fragmentShader)
{
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    //These variables store the shader code read as string from the shader source files
    std::string vs, fs;

    if (!ReadFile(vertexShader, vs)) {
        exit(1);
    };

    //Compiling vertex shader
    AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

    if (!ReadFile(fragmentShader, fs)) {
        exit(1);
    };

    //Compiling fragment shader
    AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };
    //Linking shader program
    glLinkProgram(ShaderProgram);
    //Error checking for shader linking
    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }
    //Shader program validation
    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }
    return ShaderProgram;
}