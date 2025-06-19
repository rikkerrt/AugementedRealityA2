#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <random>
#include "tigl.h"
#include "FpsCam.h"
#include "ObjModel.h"

#include "GameObject.h"
#include "SceneObject.h"
#include "CubeComponent.h"
#include "ModelComponent.h"
#include "KeyboardSteeringComponent.h"
#include "CarPhysicsComponent.h"
#include "PhysicsComponent.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "RoadComponent.h"
#include "MapLoader.h"
using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
ObjModel* model;
ObjModel* circuit;
SceneObject scene;

void init();
void initWorld();
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

    auto player = std::make_shared<GameObject>();
    player->position = glm::vec3(0, 0, 5);
    player->addComponent(std::make_shared<ModelComponent>("models/car/carNoWindow.obj"));
	player->addComponent(std::make_shared<KeyboardSteeringComponent>());
	player->addComponent(std::make_shared<CarPhysicsComponent>());
    scene.addGameObject(player);

    loadMap(&scene);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


void update()
{
    camera->update(window);
	scene.update(0.01f);
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