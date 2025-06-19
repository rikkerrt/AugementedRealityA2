#include "CarPhysicsComponent.h"
#include "GameObject.h"
#include "SteeringComponent.h"
#include "SceneObject.h"
#include "RoadComponent.h"
#include "PhysicsComponent.h"
#include "tigl.h"
#include <iostream>

CarPhysicsComponent::CarPhysicsComponent(float speed)
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

{
    this->speed = speed;
}

CarPhysicsComponent::~CarPhysicsComponent()
{
}

void CarPhysicsComponent::update(float elapsedTime)
{
    // Get desired speed
    float currentSpeed = gameObject->getComponent<SteeringComponent>()->height * this->speed;

	//looks for physics components
	std::list<std::shared_ptr<GameObject>> physicsObjects = gameObject->sceneObject->getPhysicsObjects();

	for (auto& physicsObject : physicsObjects) {
		float radius = physicsObject->getComponent<PhysicsComponent>()->radius;
		float distance = glm::length(gameObject->position - physicsObject->position);
		
		if (distance < radius) {
			float angle = glm::degrees(glm::atan(gameObject->position.x - physicsObject->position.x, gameObject->position.z - physicsObject->position.z));
			angle = glm::mod(angle + 360, 360.0f);

			float objAngle = glm::degrees(gameObject->rotation.y);

			float angleDiff = glm::mod(angle - objAngle + 540.0f, 360.0f) - 180.0f;
			if (std::abs(angleDiff) < 90.0f) {
				speed = 0.0f;
				break;
			}
		}
	}

	// looks for road
	std::list<BoundingBox*> roadBoxes = gameObject->sceneObject->getRoadBoxes();
	bool onGrass = false;
	for (auto& boundingBox : roadBoxes) {
		if (-gameObject->position.x < boundingBox->tl.x && -gameObject->position.x > boundingBox->br.x &&
			-gameObject->position.z < boundingBox->tl.y && -gameObject->position.z > boundingBox->br.y) {
			onGrass = true;
			break;
		}
	}

	if (!onGrass) {
		speed = speed / 2;
	}

    // Calculate target steering angle
    float targetAngle = glm::radians(gameObject->getComponent<SteeringComponent>()->angle);
    steeringAngle += (targetAngle - steeringAngle) * elapsedTime * 3.0f; // Gradual turning
    gameObject->rotation.y += steeringAngle * elapsedTime * 2.0f; // Smooth rotation

    // Movement based on rotation
    gameObject->position.x -= sin(gameObject->rotation.y) * currentSpeed * elapsedTime;
    gameObject->position.z -= cos(gameObject->rotation.y) * currentSpeed * elapsedTime;
}

