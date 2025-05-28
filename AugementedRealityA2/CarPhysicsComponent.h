#pragma once
#include "Component.h"

class CarPhysicsComponent : public Component
{
public:
	CarPhysicsComponent();
	~CarPhysicsComponent();
	virtual void update(float elapsedTime) override;

private:
		float steeringAngle = 0;
};