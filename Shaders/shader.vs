#version 330 core

// Positions
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Normals
layout (location = 2) in vec3 aNormal;

out vec3 color;
out vec3 Normal;
out vec3 crntPos;
uniform mat4 camMatrix;
uniform mat4 model;


//SOURCE: https://github.com/VictorGordan/opengl-tutorials/blob/main/YoutubeOpenGL%209%20-%20Lighting/default.vert
void main()
{
	crntPos = vec3(model * vec4(aPos, 1.0f));
	gl_Position = camMatrix * vec4(crntPos, 1.0);
	color = aColor;
	Normal = aNormal;
}