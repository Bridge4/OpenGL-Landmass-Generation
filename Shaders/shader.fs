#version 330 core
// Imports the color from the Vertex Shader
in vec3 color;

// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the current position from the Vertex Shader
in vec3 crntPos;

out vec4 FragColor;
// Gets the color of the light from the main function
uniform vec4 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;

void main()
{
	// ambient lighting
	float ambient = 0.1f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 1.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
    vec3 halfwayDir = normalize(lightDirection + viewDirection);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 8.0);

	// outputs final color
	FragColor = lightColor * vec4(color, 1.0) * (diffuse+ambient+spec);
}