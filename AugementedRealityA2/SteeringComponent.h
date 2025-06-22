#pragma once

#include "Component.h"

class SteeringComponent : public Component
{
public:
	SteeringComponent();
	~SteeringComponent();

	virtual void update(float elapsedTime) = 0;

	float height = .5f;
	float angle = 0;
};

