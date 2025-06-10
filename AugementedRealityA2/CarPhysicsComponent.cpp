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
    // Get desired speed
    float speed = gameObject->getComponent<SteeringComponent>()->height * 20;

    // Calculate target steering angle
    float targetAngle = glm::radians(gameObject->getComponent<SteeringComponent>()->angle);
    steeringAngle += (targetAngle - steeringAngle) * elapsedTime * 3.0f; // Gradual turning
    gameObject->rotation.y += steeringAngle * elapsedTime * 2.0f; // Smooth rotation

    // Movement based on rotation
    gameObject->position.x -= sin(gameObject->rotation.y) * speed * elapsedTime;
    gameObject->position.z -= cos(gameObject->rotation.y) * speed * elapsedTime;


}

