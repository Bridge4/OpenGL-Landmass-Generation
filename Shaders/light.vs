#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 camMatrix;
//This was inspired by the Victor Gordon github links in the other shader source files but they're extremely simple lines of code
//Was very simple to implement myself without having to reference Victor's code
void main()
{
	//applies the camera projection to the model so that when camera is moved the object is
	//transformed in such a way that it looks like a perspective POV
	gl_Position = camMatrix * model * vec4(aPos, 1.0f);
}