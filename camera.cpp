#include "camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	Up = glm::vec3(0.0f, 1.0f, 0.0f);
	//Added Horizontal vector so I could implement keyboard movement of camera
	Hor = glm::vec3(1.0f, 0.0f, 0.0f);
	cameraMatrix = glm::mat4(1.0f);
	Position = position;
	speed = 1.0f;
	sensitivity = 4.0f;
	firstClick = true;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
	//initializing matrices
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	view = glm::lookAt(Position, Position + Orientation, Up);
	//PERSPECTIVE PROJECTION!
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);
	
	cameraMatrix = projection * view;
}

void Camera::Matrix(GLuint shader, const char *uniform)
{
	// Exports camera matrix
	glUniformMatrix4fv(glGetUniformLocation(shader, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow *window)
{
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		exit(0);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -Orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		Position += speed * Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		Position += speed * -Up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 2;
	}
	//THIS IS WHAT I ADDED
	//ADDED KEYBOARD CAMERA CONTROL
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		Orientation = glm::rotate(Orientation, glm::radians(1.0f), Up);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		Orientation = glm::rotate(Orientation, glm::radians(-1.0f), Up);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(1.0f), glm::normalize(glm::cross(Orientation, Up)));
		//without validating the legality of vertical orientation you get a cone-like viewing effect
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			Orientation = newOrientation;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-1.0f), glm::normalize(glm::cross(Orientation, Up)));

		// Decides whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
			Orientation = newOrientation;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 1.0f;
	}

	// Handles mouse inputs
	//MOUSE INPUTS DONE BY VICTOR NOT ME BUT THEY FOLLOW ESSENTIALLY WHAT I DID FOR THE FPS HOMEWORK
	//https://github.com/VictorGordan/opengl-tutorials/tree/main/YoutubeOpenGL%208%20-%20Camera
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		double mouseX;
		double mouseY;
		//Get cursor position
		glfwGetCursorPos(window, &mouseX, &mouseY);
		std::cout << mouseX << std::endl;
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));
		//Rotates up and down
		if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			Orientation = newOrientation;
		}

		//Rotates left and right
		Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);
	}
}