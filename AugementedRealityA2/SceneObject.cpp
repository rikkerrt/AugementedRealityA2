#include "SceneObject.h"
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include "GameObject.h"
#include "PhysicsComponent.h"

#include <iostream>


SceneObject::SceneObject() 
{
}

SceneObject::~SceneObject() 
{
}

void SceneObject::addGameObject(std::shared_ptr<GameObject> gameObject)
{
	gameObject->setSceneObject(this);
	gameObjects.push_back(gameObject);
}

void SceneObject::addRoadObject(std::shared_ptr<GameObject> gameObject, int type)
{
	gameObject->setSceneObject(this);

	gameObject->position = glm::vec3(x, 0.0, y);
	gameObject->rotation = glm::vec3(0.0, rotationY, 0.0);
	if (type == 1) {
		x += 0.0 * cos(rotationY) - 10.5475 * sin(rotationY);
		y -= 0.0 * sin(rotationY) + 10.5475 * cos(rotationY);
	}
	if (type == 2) {
		rotationY -= glm::radians(11.25);
		x += -0.2 * cos(rotationY) - 2.05 * sin(rotationY);
		y -= -0.2 * sin(rotationY) + 2.05 * cos(rotationY);
	}
	if (type == 3) {
		rotationY += glm::radians(11.25);
		x += 0.2 * cos(rotationY) - 2.05 * sin(rotationY);
		y -= 0.2 * sin(rotationY) + 2.05 * cos(rotationY);
	}
	if (type == 4) {
		x += 0.0 * cos(rotationY) - 4.224 * sin(rotationY);
		y -= 0.0 * sin(rotationY) + 4.224 * cos(rotationY);
	}
	gameObjects.push_back(gameObject);

	std::cout << "oui oui" << x << ", " << y << std::endl;
	
}

std::list<std::shared_ptr<GameObject>> SceneObject::getGameObjects()
{
	return gameObjects;
}

std::list<std::shared_ptr<GameObject>> SceneObject::getPhysicsObjects()
{
	std::list<std::shared_ptr<GameObject>> physicsObjects;
	for (auto& gameObject : gameObjects) {
		if (gameObject->getComponent<PhysicsComponent>()) {
			physicsObjects.push_back(gameObject);
		}
	}
	return physicsObjects;
}

void SceneObject::update(float elapsedTime)
{
	for (auto& gameObject : gameObjects) {
		gameObject->update(elapsedTime);
	}
}

void SceneObject::draw(const glm::mat4& parentMatrix)
{
	for (auto& gameObject : gameObjects) {
		gameObject->draw(parentMatrix);
	}
}