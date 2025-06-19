#include "CameraManager.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include <glm/glm.hpp>


CameraManager::CameraManager(std::shared_ptr<GameObject> cameraObject)
{
    this->activeCameraObject = cameraObject;
}

void CameraManager::setActiveCamera(std::shared_ptr<GameObject> cameraObject) {
    activeCameraObject = cameraObject;
}

CameraComponent* CameraManager::getCameraComponent() {
    return activeCameraObject ? activeCameraObject->getComponent<CameraComponent>().get() : nullptr;
}

glm::mat4 CameraManager::getViewMatrix()  {
    auto camera = getCameraComponent();
    return camera ? camera->getViewMatrix() : glm::mat4(1.0f);
}

glm::mat4 CameraManager::getProjectionMatrix(float aspectRatio) {
    auto camera = getCameraComponent();
    return camera ? camera->getProjectionMatrix(aspectRatio) : glm::mat4(1.0f);
}
