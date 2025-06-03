#pragma once
#include <list>
#include <glm/glm.hpp>
#include <memory>


class GameObject;

class SceneObject
{

	std::list<std::shared_ptr<GameObject>> gameObjects;
	float x = 0.0f;
	float y = 0.0f;
	float rotationY = 0.0f;

public:
	SceneObject();
	~SceneObject();
	
	void addGameObject(std::shared_ptr<GameObject> gameObject);
	void addRoadObject(std::shared_ptr<GameObject> gameObject, int type);
	std::list<std::shared_ptr<GameObject>> getGameObjects();
	void update(float elapsedTime);
	void draw(const glm::mat4& parentMatrix = glm::mat4(1.0f));



};