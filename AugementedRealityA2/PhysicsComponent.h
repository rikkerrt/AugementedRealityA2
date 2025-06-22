#pragma once
#include "Component.h"

class PhysicsComponent : public Component
{
public:
	float radius;

	PhysicsComponent(float inRadius);
	~PhysicsComponent();
};
