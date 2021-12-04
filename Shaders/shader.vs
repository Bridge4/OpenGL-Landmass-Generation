#version 330 core

// Positions
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Normals
layout (location = 2) in vec3 aNormal;

// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the current position for the Fragment Shader
out vec3 crntPos;

// Imports the camera matrix from the main function
uniform mat4 camMatrix;
// Imports the model matrix from the main function
uniform mat4 model;

vec3 getNormal(vec3 aPos)
{
	vec3 A = vec3(aPos.x, 0, 0);
    vec3 B = vec3(0, aPos.y, 0);
    vec3 C = vec3(0, 0, aPos.z);

    //From my lighting homework

    //  VECTOR FROM A TO B
    float dx0 = A.x - B.x;
    float dy0 = A.y - B.y;
    float dz0 = A.z - B.z;
    //  VECTOR FROM C TO A
    float dx1 = C.x - A.x;
    float dy1 = C.y - A.y;
    float dz1 = C.z - A.z;
    //  Normal
    float Nx = dy0 * dz1 - dy1 * dz0;
    float Ny = dz0 * dx1 - dz1 * dx0;
    float Nz = dx0 * dy1 - dx1 * dy0;

	vec3 normal = vec3(Nx, Ny, Nz);
	return normal;
}

void main()
{
	// calculates current position
	crntPos = vec3(model * vec4(aPos, 1.0f));
	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * vec4(crntPos, 1.0);

	// Assigns the colors from the Vertex Data to "color"
	color = aColor;

	// Assigns the normal from the Vertex Data to "Normal"
	//Normal = getNormal(aPos);
	Normal = aNormal;
}