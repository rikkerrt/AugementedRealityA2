#pragma once
#include <list>
#include <glm/glm.hpp>
#include "RoadComponent.h"
#include <memory>
#include <vector>
#include "CheckPointManager.h"

class GameObject;

class SceneObject
{

	std::list<std::shared_ptr<GameObject>> gameObjects;
	double x = 0.0;
	double y = 0.0;
	double rotationY = 0.0;
	std::vector<CheckPoint> checkPoints;
	void createCheckPointFromBoundingBox(RoadComponent::BoundingBox* box, ZoneType type);

public:
	SceneObject();
	~SceneObject();
	
	void addGameObject(std::shared_ptr<GameObject> gameObject);
	void addRoadObject(std::shared_ptr<GameObject> gameObject, int type);
	std::list<std::shared_ptr<GameObject>> getGameObjects();
	std::list<std::shared_ptr<GameObject>> getPhysicsObjects();
	std::list<RoadComponent::BoundingBox*> getRoadBoxes();
	void update(float elapsedTime);
	void draw(const glm::mat4& parentMatrix = glm::mat4(1.0f));
	std::vector<CheckPoint> getCheckPoints();


};