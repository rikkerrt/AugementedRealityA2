#include "ChildComponent.h"
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>

using namespace glm;

ChildComponent::ChildComponent(std::shared_ptr<GameObject> parent, vec3 postionOffset, vec3 rotationOffset, vec3 scaleOffset)
{
	this->parent = parent;
	this->postionOffset = postionOffset;
	this->rotationOffset = rotationOffset;
	this->scaleOffset = scaleOffset;
}

ChildComponent::~ChildComponent()
{

}

void ChildComponent::update(float elapsedTime)
{
    glm::mat4 rotationMatrix = glm::yawPitchRoll(
        parent->rotation.y,
        parent->rotation.x,
        parent->rotation.z
    );

    glm::vec3 rotatedOffset = glm::vec3(rotationMatrix * glm::vec4(postionOffset, 1.0f));

    this->gameObject->position = parent->position + rotatedOffset;
    this->gameObject->rotation = parent->rotation + rotationOffset;
    this->gameObject->scale = parent->scale + scaleOffset;
}
