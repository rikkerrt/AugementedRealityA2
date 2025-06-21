#pragma once

#include "SteeringComponent.h"

class KeyboardSteeringComponent : public SteeringComponent
{
	float angularVelocity = 180.0f;
	float velocity = 0.1f;
public:
	KeyboardSteeringComponent();
	~KeyboardSteeringComponent();

	virtual void update(float elapsedTime) override;
};

