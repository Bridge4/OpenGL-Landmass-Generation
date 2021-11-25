#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;

// Outputs the color for the Fragment Shader
out vec3 color;

// Imports the camera matrix from the main function
uniform mat4 camMatrix;


void main()
{
	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * vec4(aPos, 1.0);
	if(aPos.z < 0)
	{
		color = vec3(0.133,0.18,0.314);
	}
	else if(aPos.z > 25)
	{
		color = vec3(0.91,0.882,0.937);
	}
	else if(aPos.z < 5 && aPos.z > 0)
	{
		color = vec3(0.922,0.988,0.984);
	}
	else
	{
		color = vec3(0.286,0.627,0.471);
	}
}