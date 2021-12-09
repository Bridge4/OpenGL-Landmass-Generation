#version 330 core
out vec4 FragColor;

uniform vec4 lightColor;

void main()
{
	//All this does is make the cube white
	FragColor = lightColor;
}