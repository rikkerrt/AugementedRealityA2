#include "CarPhysicsComponent.h"
#include "GameObject.h"
#include "SteeringComponent.h"
#include "SceneObject.h"
#include "RoadComponent.h"
#include "PhysicsComponent.h"
#include "tigl.h"
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

CarPhysicsComponent::CarPhysicsComponent()
{
}

CarPhysicsComponent::~CarPhysicsComponent()
{
}

void CarPhysicsComponent::update(float elapsedTime)
{
	float speed = gameObject->getComponent<SteeringComponent>()->speed * elapsedTime;
	float angle = gameObject->getComponent<SteeringComponent>()->angle;

	//looks for physics components
	std::list<std::shared_ptr<GameObject>> physicsObjects = gameObject->sceneObject->getPhysicsObjects();

	for (auto& physicsObject : physicsObjects) {
		float radius = physicsObject->getComponent<PhysicsComponent>()->radius;
		float distance = glm::length(gameObject->position - physicsObject->position);

		float angle = glm::degrees(glm::atan(gameObject->position.z - physicsObject->position.z, gameObject->position.x - physicsObject->position.x) + glm::radians(90.0f)) + 180;

		float twoPI = glm::pi<float>() * 2.0f;
		float objAngle = glm::degrees(std::fmod(std::fmod(gameObject->rotation.y, twoPI) + twoPI, twoPI));

		std::cout << "Collision with physics object detected at angle: " << angle << ", " << objAngle << " degrees" << std::endl;
		
		if (distance < radius) {
			speed = 0.0f;
			//std::cout << angle + glm::radians(90.0f) << ", " << angle - glm::radians(90.0f) << std::endl;
			/*if (objAngle < angle + glm::radians(90.0f) && objAngle > angle - glm::radians(90.0f)) {
				std::cout << "Collision with physics object detected!" << std::endl;
				speed = 0.0f;
			}
			else {
				std::cout << "No collision with physics object detected!" << std::endl;
			}*/
			//std::cout << "Collision with physics object detected!" << std::endl;
			break;
		}
		else {
			//std::cout << "No collision with physics object detected!" << std::endl;
		}
	}

	// looks for road
	std::list<BoundingBox*> roadBoxes = gameObject->sceneObject->getRoadBoxes();
	bool ez = false;
	for (auto& boundingBox : roadBoxes) {
		if (-gameObject->position.x < boundingBox->tl.x && -gameObject->position.x > boundingBox->br.x &&
			-gameObject->position.z < boundingBox->tl.y && -gameObject->position.z > boundingBox->br.y) {
			//std::cout << "Collision with road detected!" << std::endl;
			ez = true;
			break;
		}
	}

	if (!ez) {
		//std::cout << "No collision with road detected!" << std::endl;
		//std::cout << "Camera Position: " << gameObject->position.x << ", " << gameObject->position.z << std::endl;
		speed = speed / 2;
	}

	gameObject->position.x += (float)cos(glm::radians(angle - 90)) * speed;
	gameObject->position.z += (float)sin(glm::radians(angle - 90)) * speed;
	// angle draaien fixen door de rotatie van de cubes hetzelfde te maken als de angle waar hij naartoe rijd
	//gameObject->rotation.y = (angle / 90) * -1;
	// angle to radian
	gameObject->rotation.y = glm::radians(angle) * -1; // -1 because of the way the car is facing
}
