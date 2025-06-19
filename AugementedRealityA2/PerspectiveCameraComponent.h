#pragma once
#include "CameraComponent.h"
#include "GameObject.h"

class PerspectiveCameraComponent : public CameraComponent
{
private:
	float fov;
	float nearClip;
	float farClip;

public:
	PerspectiveCameraComponent(float fov = 75.0f, float nearClip = 0.01f, float farClip = 200.0f);
	~PerspectiveCameraComponent();

	glm::mat4 getViewMatrix() override;
	glm::mat4 getProjectionMatrix(float aspect) override; // TODO better way to get aspect?
};

