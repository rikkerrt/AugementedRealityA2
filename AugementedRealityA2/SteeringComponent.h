#pragma once

#include "Component.h"

class SteeringComponent : public Component
{
public:
	float speed;
	float angle;

	SteeringComponent();
	~SteeringComponent();
	virtual void update(float elapsedTime) = 0;
};
