#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H
#include "CSCIx229.h"
//camera class sourced from https://github.com/VictorGordan/opengl-tutorials/tree/main/YoutubeOpenGL%208%20-%20Camera

class Camera
{
public:
	/*
		These values were originally initialized here but I moved it to the constructor to prevent
		C++11 warnings
	*/
	//Vectors determining positions, orientation of camera
	glm::vec3 Position;
	glm::vec3 Orientation;
	glm::vec3 Up;
	glm::vec3 Hor;
	/*
		Camera matrix thats exported to the shader when Matrix() is called so that scene can be 
		transformed
	*/
	glm::mat4 cameraMatrix;
	bool firstClick;
	// Stores the width and height of the window
	int width;
	int height;
	// Adjust the speed of the camera and it's sensitivity when looking around
	float speed;
	float sensitivity;
	// Camera constructor to set up initial size and position of camera
	Camera(int width, int height, glm::vec3 position);
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	// Exports the camera matrix to a shader
	void Matrix(GLuint shader, const char* uniform);
	// Handles camera inputs
	void Inputs(GLFWwindow* window);
};
#endif