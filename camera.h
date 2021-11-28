#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H
#include "CSCIx229.h"
//camera class sourced from https://github.com/VictorGordan/opengl-tutorials/tree/main/YoutubeOpenGL%208%20-%20Camera

class Camera
{
public:
	// Stores the main vectors of the camera
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 Hor = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);
	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;

	// Stores the width and height of the window
	int width;
	int height;

	// Adjust the speed of the camera and it's sensitivity when looking around
	float speed = 1.0f;
	float sensitivity = 4.0f;

	// Camera constructor to set up initial values
	Camera(int width, int height, glm::vec3 position);
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	// Exports the camera matrix to a shader
	void Matrix(GLuint shader, const char* uniform);
	// Handles camera inputs
	void Inputs(GLFWwindow* window);
};
#endif