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
	float speed = gameObject->getComponent<SteeringComponent>()->speed * elapsedTime;
	float angle = gameObject->getComponent<SteeringComponent>()->angle - 90;

	gameObject->position.x += (float)cos(glm::radians(angle)) * speed;
	gameObject->position.z += (float)sin(glm::radians(angle)) * speed;
	// angle draaien fixen door de rotatie van de cubes hetzelfde te maken als de angle waar hij naartoe rijd
	gameObject->rotation.y = (angle / 90) * -1;
}
