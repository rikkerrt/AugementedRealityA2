#pragma once

#include "SteeringComponent.h"

class KeyboardSteeringComponent : public SteeringComponent
{
public:
	KeyboardSteeringComponent();
	~KeyboardSteeringComponent();

	virtual void update(float elapsedTime) override;
};

