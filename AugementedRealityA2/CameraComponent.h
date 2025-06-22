#pragma once
#include "Component.h"
#include <glm/glm.hpp>

class CameraComponent : public Component
{
public:
    virtual glm::mat4 getViewMatrix() = 0;
    virtual glm::mat4 getProjectionMatrix(float aspect) = 0;
};