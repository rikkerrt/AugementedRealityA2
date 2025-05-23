#pragma once
#include "Component.h"
#include <GLFW/glfw3.h>

class PlayerComponent : public Component
{
	float speed = 1.0f;
public:
	PlayerComponent();
	~PlayerComponent();

	virtual void update(float elapsedTime) override;

};