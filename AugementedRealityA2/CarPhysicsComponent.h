#pragma once
#include "Component.h"

class CarPhysicsComponent : public Component
{
public:
	CarPhysicsComponent(float speed);
	~CarPhysicsComponent();
	virtual void update(float elapsedTime) override;

private:
		float steeringAngle = 0;
		float speed = 0;
};