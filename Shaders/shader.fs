#version 330 core

//Inputs
in vec3 color;
in vec3 Normal;
in vec3 crntPos;

//Outputs
out vec4 FragColor;

//Uniforms
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

//Followed this tutorial: https://learnopengl.com/Advanced-Lighting/Advanced-Lighting 
//Also added to with the halfway direction vector: https://github.com/VictorGordan/opengl-tutorials/blob/main/YoutubeOpenGL%209%20-%20Lighting/default.vert
void main()
{
	//AMBIENT
	float ambient = 0.15f;

	//DIFFUSE
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0);

	//SPECULAR
	float specularIntensity = 0.2f;
	vec3 viewDirection = normalize(camPos - crntPos);
	/*
		This is what differentiates Phong lighting from Blinn Phong Lighting
		The closer the camera is to the original reflection direction, the more intense the specular effect
	*/
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float spec = pow(max(dot(normal, halfwayDirection), 0.0), 8.0);
	float specular = spec * specularIntensity;
	// Blinn Phong Lighting 
	FragColor = lightColor * vec4(color, 1.0) * (diffuse+ambient+specular);
	//FragColor = vec4(color, 1.0);
}
