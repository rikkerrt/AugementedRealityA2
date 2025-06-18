#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include "FpsCam.h"
#include "ObjModel.h"

#include "GameObject.h"
#include "SceneObject.h"
#include "CubeComponent.h"
#include "ModelComponent.h"
#include "KeyboardSteeringComponent.h"
#include "CarPhysicsComponent.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "RoadComponent.h"
using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
ObjModel* model;
ObjModel* circuit;
SceneObject scene;

void init();
void update();
void draw();
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void)
{
    if (!glfwInit())
        throw "Could not initialize glwf";
    int count;
    window = glfwCreateWindow(1000, 800, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glwf";
    }
    glfwMakeContextCurrent(window);

    tigl::init();

    init();

    while (!glfwWindowShouldClose(window))
    {
        update();
        draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();


    return 0;
}

FpsCam* camera;

void init()
{

    int value[10];
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, value);
    glfwSetKeyCallback(window, keyCallback);

    camera = new FpsCam(window);

    tigl::shader->enableTexture(true);

    scene = SceneObject();

    /*auto player = std::make_shared<GameObject>();
    player->position = glm::vec3(0, 0, 5);
    player->addComponent(std::make_shared<ModelComponent>("models/car/carNoWindow.obj"));
	player->addComponent(std::make_shared<KeyboardSteeringComponent>());
	player->addComponent(std::make_shared<CarPhysicsComponent>());
    scene.addGameObject(player);

	auto circuit = std::make_shared<GameObject>();
	circuit->position = glm::vec3(0, 0, 0);
	circuit->addComponent(std::make_shared<ModelComponent>("models/circuit/circuit.obj"));
    scene.addGameObject(circuit);*/

    auto groundLayer = std::make_shared<GameObject>();
    groundLayer->position = glm::vec3(0, -0.2, 0);
    groundLayer->addComponent(std::make_shared<ModelComponent>("models/test/GroundLayer/GroundLayer.obj"));
    scene.addGameObject(groundLayer);

    auto straight2 = std::make_shared<GameObject>();
    straight2->position = glm::vec3(0, 0, 0);
    straight2->addComponent(std::make_shared<ModelComponent>("models/test/straight/Curve.obj"));
    scene.addRoadObject(straight2, 1);

	/*auto straight1 = std::make_shared<GameObject>();
	straight1->position = glm::vec3(0, 0, 0);
	straight1->addComponent(std::make_shared<ModelComponent>("models/test/timing/StartFinish.obj"));
	scene.addRoadObject(straight1, 4);*/

    auto sectorStraight = std::make_shared<GameObject>();
    sectorStraight->position = glm::vec3(0, 0, 0);
    sectorStraight->addComponent(std::make_shared<ModelComponent>("models/test/timing/SectorStraight.obj"));
    scene.addGameObject(sectorStraight);

	auto prop = std::make_shared<GameObject>();
	prop->position = glm::vec3(0, 0, 0);
	prop->addComponent(std::make_shared<ModelComponent>("models/test/props/Tree1.obj"));
	scene.addGameObject(prop);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_V && action == GLFW_PRESS) {
		auto straight3 = std::make_shared<GameObject>();
		straight3->position = glm::vec3(0, 0, 0);
		straight3->addComponent(std::make_shared<ModelComponent>("models/test/straight/curve.obj"));
		scene.addRoadObject(straight3, 1);
	}
	if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		auto straight2 = std::make_shared<GameObject>();
		straight2->position = glm::vec3(0, 0, 0);
		straight2->addComponent(std::make_shared<ModelComponent>("models/test/corner/CurveRight.obj"));
		scene.addRoadObject(straight2, 2);
	}

	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		auto straight4 = std::make_shared<GameObject>();
		straight4->position = glm::vec3(0, 0, 0);
		straight4->addComponent(std::make_shared<ModelComponent>("models/test/corner/CurveLeft.obj"));
		scene.addRoadObject(straight4, 3);
	}

    if (key == GLFW_KEY_K && action == GLFW_PRESS) {
        auto sectorStraight = std::make_shared<GameObject>();
        sectorStraight->position = glm::vec3(0, 0, 0);
        sectorStraight->addComponent(std::make_shared<ModelComponent>("models/test/timing/SectorStraight.obj"));
        scene.addRoadObject(sectorStraight,5);
    }
}


void update()
{
    camera->update(window);
	scene.update(0.01f);

    std::list<BoundingBox*> roadBoxes = scene.getRoadBoxes();
	bool ez = false;
    for (auto& boundingBox : roadBoxes) {
        if (camera->position.x < boundingBox->tl.x && camera->position.x > boundingBox->br.x &&
            camera->position.z < boundingBox->tl.y && camera->position.z > boundingBox->br.y) {
            std::cout << "Collision with road detected!" << std::endl;
			ez = true;
            return; // Collision detected, stop the car
        }
    }
    if (!ez) {
        std::cout << "No collision with road detected!" << std::endl;
        std::cout << "Camera Position: " << camera->position.x << ", " << camera->position.z << std::endl;
    }
}

void draw()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), viewport[2] / (float)viewport[3], 0.01f, 200.0f);

    tigl::shader->setProjectionMatrix(projection);
    tigl::shader->setViewMatrix(camera->getMatrix());
    tigl::shader->setModelMatrix(glm::mat4(1.0f));

    tigl::shader->enableColor(true);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE);

    glPointSize(10.0f);

	scene.draw();
}