#include "KeyboardSteeringComponent.h"
#include "GameObject.h"
#include <GLFW/glfw3.h>

extern GLFWwindow* window;

KeyboardSteeringComponent::KeyboardSteeringComponent() {
	speed = 0.0f;
	angle = 0.0f;
}

KeyboardSteeringComponent::~KeyboardSteeringComponent() {

}

void KeyboardSteeringComponent::update(float elapsedTime) {
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		angle -= 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		angle += 1.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		speed += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		speed -= 0.1f;
	}

	if (speed > 10.0f)
		speed = 10.0f;
	if (speed < -10.0f)
		speed = -10.0f;
}