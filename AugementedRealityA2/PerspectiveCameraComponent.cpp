#pragma once

#include "Component.h"
#include "PerspectiveCameraComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

PerspectiveCameraComponent::PerspectiveCameraComponent(float fov, float nearClip, float farClip)
{
    this->fov = fov;
    this->nearClip = nearClip;
    this->farClip = farClip;
}

PerspectiveCameraComponent::~PerspectiveCameraComponent()
{

}

glm::mat4 PerspectiveCameraComponent::getViewMatrix() {
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, gameObject->position);
    transform = glm::rotate(transform, gameObject->rotation.x, glm::vec3(1, 0, 0));
    transform = glm::rotate(transform, gameObject->rotation.y, glm::vec3(0, 1, 0));
    transform = glm::rotate(transform, gameObject->rotation.z, glm::vec3(0, 0, 1));
    return glm::inverse(transform);
}

glm::mat4 PerspectiveCameraComponent::getProjectionMatrix(float aspect) {
    return glm::perspective(glm::radians(fov), aspect, nearClip, farClip);
}