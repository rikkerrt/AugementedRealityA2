#pragma once
#include "CameraComponent.h"
#include "GameObject.h"

class CameraManager 
{
    
    std::shared_ptr<GameObject> activeCameraObject = nullptr;

public:
    CameraManager(std::shared_ptr<GameObject> cameraManager);
    
    void setActiveCamera(std::shared_ptr<GameObject> cameraObject);

    CameraComponent* getCameraComponent();

    glm::mat4 getViewMatrix();

    glm::mat4 getProjectionMatrix(float aspectRatio);
};
