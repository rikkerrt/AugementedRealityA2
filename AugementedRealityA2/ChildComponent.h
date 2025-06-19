#pragma once

#include "Component.h"
#include "GameObject.h"
#include <glm/glm.hpp>

using namespace glm;

class ChildComponent : public Component
{
private:
	std::shared_ptr<GameObject> parent;
	vec3 postionOffset;
	vec3 rotationOffset;
	vec3 scaleOffset;

public:
	ChildComponent(std::shared_ptr<GameObject> parent, vec3 postionOffset = vec3{0, 0, 0}, vec3 rotationOffset = vec3{0, 0, 0}, vec3 scaleOffset = vec3{0, 0, 0});
	~ChildComponent();

	void update(float elapsedTime) override;
};

