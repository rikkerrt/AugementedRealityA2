#include "KeyboardSteeringComponent.h"
#include "GameObject.h"
#include <GLFW/glfw3.h>
#include <iostream>

extern GLFWwindow* window;


KeyboardSteeringComponent::KeyboardSteeringComponent()
{
}

KeyboardSteeringComponent::~KeyboardSteeringComponent()
{
}

void KeyboardSteeringComponent::update(float elapsedTime)
{

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		float height = this->height + (elapsedTime * velocity);
		this->height = height > 1 ? 1 : height;

	}

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		float height = this->height - (elapsedTime * velocity);
		this->height = height < 0 ? 0 : height;
	}

	// TODO not sure if the angle should be between 0 and 180 and 0 and - 180
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		float angle = this->angle + (elapsedTime * angularVelocity);
		this->angle = angle > 180 ? -180 : angle;
	}

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		float angle = this->angle - (elapsedTime * angularVelocity);
		this->angle = angle < -180 ? 180 : angle;
	}

	std::cout << "Height (keyboard): " << height << std::endl;
	std::cout << "Angle (keyboard): " << angle << std::endl;
}
