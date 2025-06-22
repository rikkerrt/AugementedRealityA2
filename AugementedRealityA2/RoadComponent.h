#pragma once
#include "Component.h"
#include <glm/glm.hpp>

class RoadComponent : public Component
{

public:
	struct Rect {
		glm::vec2 tl, tr, br, bl;
	};

	struct BoundingBox {
		glm::vec2 tl, br;
	};

	BoundingBox* roadBox;

	RoadComponent(BoundingBox* rect);
	~RoadComponent();

};
