#pragma once

#include "Component.h"
#include <vector>
#include "tigl.h"

class DrawComponent : public Component
{
public:
	DrawComponent();
	~DrawComponent();
	virtual void draw() = 0;
};