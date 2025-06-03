#include "SceneObject.h"
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include "GameObject.h"


SceneObject::SceneObject() 
{
}

SceneObject::~SceneObject() 
{
}

void move(float angle, float fac)
{
	float x = (float)cos(glm::radians(angle)) * fac;
	float z = (float)sin(glm::radians(angle)) * fac;
}

void SceneObject::addGameObject(std::shared_ptr<GameObject> gameObject)
{
	gameObject->setSceneObject(this);
	gameObjects.push_back(gameObject);
}

void SceneObject::addRoadObject(std::shared_ptr<GameObject> gameObject, int type)
{
	gameObject->setSceneObject(this);

	gameObject->position = glm::vec3(x, 0.0f, y);
	gameObject->rotation = glm::vec3(0.0f, rotationY, 0.0f);
	if (type == 1) {
		x += 0.0f * cos(rotationY) - 10.5475f * sin(rotationY);
		y -= 0.0f * sin(rotationY) + 10.5475f * cos(rotationY);
	}
	if (type == 2) {
		x += 0.2f * cos(rotationY) - 2.05f * sin(rotationY);
		y -= 0.2f * sin(rotationY) + 2.05f * cos(rotationY);

		rotationY -= glm::radians(11.25f);
	}
	gameObjects.push_back(gameObject);
}

std::list<std::shared_ptr<GameObject>> SceneObject::getGameObjects()
{
	return gameObjects;
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