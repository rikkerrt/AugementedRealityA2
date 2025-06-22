#include "SceneObject.h"
#include "tigl.h"
#include <glm/gtc/matrix_transform.hpp>
#include "GameObject.h"
#include "PhysicsComponent.h"
#include "RoadComponent.h"
#include "ModelComponent.h"

#include <algorithm>
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

std::vector<Vertex> verts;

void SceneObject::addRoadObject(std::shared_ptr<GameObject> gameObject, int type)
{
	gameObject->setSceneObject(this);

	gameObject->position = glm::vec3(x, 0.0, y);
	gameObject->rotation = glm::vec3(0.0, rotationY, 0.0);

	//gets object model component
	RoadComponent::BoundingBox* boundingBox = new RoadComponent::BoundingBox();
	RoadComponent::Rect* modelBox = gameObject->getComponent<ModelComponent>()->getModel()->getRect();
	RoadComponent::Rect* translatedBox = new RoadComponent::Rect();

	translatedBox->tl = glm::vec2(
		-(modelBox->tl.x * cos(rotationY) - modelBox->tl.y * sin(rotationY)) - x,
		(modelBox->tl.x * sin(rotationY) + modelBox->tl.y * cos(rotationY)) - y
	);
	translatedBox->tr = glm::vec2(
		-(modelBox->tr.x * cos(rotationY) - modelBox->tr.y * sin(rotationY)) - x,
		(modelBox->tr.x * sin(rotationY) + modelBox->tr.y * cos(rotationY)) - y
	);
	translatedBox->bl = glm::vec2(
		-(modelBox->bl.x * cos(rotationY) - modelBox->bl.y * sin(rotationY)) - x,
		(modelBox->bl.x * sin(rotationY) + modelBox->bl.y * cos(rotationY)) - y
	);
	translatedBox->br = glm::vec2(
		-(modelBox->br.x * cos(rotationY) - modelBox->br.y * sin(rotationY)) - x,
		(modelBox->br.x * sin(rotationY) + modelBox->br.y * cos(rotationY)) - y
	);

	boundingBox->tl.x = std::max(std::max(std::max(translatedBox->tl.x, translatedBox->tr.x), translatedBox->bl.x), translatedBox->br.x);
	boundingBox->tl.y = std::max(std::max(std::max(translatedBox->tl.y, translatedBox->tr.y), translatedBox->bl.y), translatedBox->br.y);

	boundingBox->br.x = std::min(std::min(std::min(translatedBox->tl.x, translatedBox->tr.x), translatedBox->bl.x), translatedBox->br.x);
	boundingBox->br.y = std::min(std::min(std::min(translatedBox->tl.y, translatedBox->tr.y), translatedBox->bl.y), translatedBox->br.y);

	gameObject->addComponent(std::make_shared<RoadComponent>(boundingBox));

	if (type == 1) 
	{
		x += 0.0 * cos(rotationY) - 10.5475 * sin(rotationY);
		y -= 0.0 * sin(rotationY) + 10.5475 * cos(rotationY);
	}
	if (type == 2) 
	{
		rotationY -= glm::radians(11.25);
		x += -0.2 * cos(rotationY) - 2.05 * sin(rotationY);
		y -= -0.2 * sin(rotationY) + 2.05 * cos(rotationY);
	}
	if (type == 3) 
	{
		rotationY += glm::radians(11.25);
		x += 0.2 * cos(rotationY) - 2.05 * sin(rotationY);
		y -= 0.2 * sin(rotationY) + 2.05 * cos(rotationY);
	}
	if (type == 4) 
	{
		createCheckPointFromBoundingBox(boundingBox, ZoneType::Start);

		x += 0.0 * cos(rotationY) - 4.224 * sin(rotationY);
		y -= 0.0 * sin(rotationY) + 4.224 * cos(rotationY);
	}
	if (type == 5) 
	{
		createCheckPointFromBoundingBox(boundingBox, ZoneType::Checkpoint);

		x += 0.0 * cos(rotationY) - 10.5475 * sin(rotationY);
		y -= 0.0 * sin(rotationY) + 10.5475 * cos(rotationY);
	}
	gameObjects.push_back(gameObject);
}

std::list<std::shared_ptr<GameObject>> SceneObject::getGameObjects()
{
	return gameObjects;
}

std::list<std::shared_ptr<GameObject>> SceneObject::getPhysicsObjects()
{
	std::list<std::shared_ptr<GameObject>> physicsObjects;
	for (auto& gameObject : gameObjects) 
	{
		if (gameObject->getComponent<PhysicsComponent>()) 
		{
			physicsObjects.push_back(gameObject);
		}
	}
	return physicsObjects;
}

std::list<RoadComponent::BoundingBox*> SceneObject::getRoadBoxes()
{
	std::list<RoadComponent::BoundingBox*> roadBoxes;
	for (auto& gameObject : gameObjects) 
	{
		if (gameObject->getComponent<RoadComponent>()) 
		{
			roadBoxes.push_back(gameObject->getComponent<RoadComponent>()->roadBox);
		}
	}
	return roadBoxes;
}

void SceneObject::update(float elapsedTime)
{
	for (auto& gameObject : gameObjects) 
	{
		gameObject->update(elapsedTime);
	}
}

void SceneObject::draw(const glm::mat4& parentMatrix)
{
	for (auto& gameObject : gameObjects) 
	{
		gameObject->draw(parentMatrix);
	}

	std::vector<tigl::Vertex> verts;
	std::list<RoadComponent::BoundingBox*> roadBoxes = getRoadBoxes();

	tigl::shader->setModelMatrix(glm::mat4(1.0f));
	/*for (auto& roadBox : roadBoxes) {
		verts.push_back(tigl::Vertex::P(glm::vec3(-roadBox->tl.x, 5.0f, -roadBox->tl.y)));
		verts.push_back(tigl::Vertex::P(glm::vec3(-roadBox->br.x, 5.0f, -roadBox->tl.y)));
		verts.push_back(tigl::Vertex::P(glm::vec3(-roadBox->br.x, 5.0f, -roadBox->br.y)));
		verts.push_back(tigl::Vertex::P(glm::vec3(-roadBox->tl.x, 5.0f, -roadBox->br.y)));
	}

	tigl::drawVertices(GL_QUADS, verts);*/
}

std::vector<CheckPoint> SceneObject::getCheckPoints()
{
	return checkPoints;
}

void SceneObject::createCheckPointFromBoundingBox(RoadComponent::BoundingBox* box, ZoneType type)
{
	// Draai eerst x en z richting om
	float x1 = -box->tl.x;
	float z1 = -box->tl.y;
	float x2 = -box->br.x;
	float z2 = -box->br.y;

	CheckPoint cp;
	cp.min = glm::vec3(std::min(x1, x2), 0.0f, std::min(z1, z2));
	cp.max = glm::vec3(std::max(x1, x2), 0.0f, std::max(z1, z2));
	cp.type = type;
	cp.rotation = glm::vec3(0.0f, rotationY, 0.0f);

	checkPoints.push_back(cp);
}
