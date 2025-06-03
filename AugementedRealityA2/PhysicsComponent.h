#pragma once
#include "Component.h"

class CarPhysicsComponent : public Component
{
public:
	float radius;


	CarPhysicsComponent(float inRadius);
	~CarPhysicsComponent();
};
