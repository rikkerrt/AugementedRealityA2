#include "CarPhysicsComponent.h"
#include "GameObject.h"
#include "SteeringComponent.h"
#include <iostream>

CarPhysicsComponent::CarPhysicsComponent()
{
}

CarPhysicsComponent::~CarPhysicsComponent()
{
}

void CarPhysicsComponent::update(float elapsedTime)
{
	float speed = gameObject->getComponent<SteeringComponent>()->height;
	float targetAngle = glm::radians(gameObject->getComponent<SteeringComponent>()->angle);
	//float deltaAngle = targetAngle - gameObject->rotation.y;

	//steeringAngle += deltaAngle * elapsedTime * 5; // TODO magic 5
	//gameObject->rotation.y += steeringAngle * elapsedTime * 5; // TODO magic 5

	//gameObject->position.x += sin(gameObject->rotation.y) * speed; // TODO += - speed is messy
	//gameObject->position.z += cos(gameObject->rotation.y) * speed;;

	gameObject->position.x += -(float)sin(targetAngle) * speed; // TODO += - speed is messy
	gameObject->position.z += -(float)cos(targetAngle) * speed;

	gameObject->rotation.y += targetAngle;
}
