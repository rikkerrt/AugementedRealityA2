#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include "FpsCam.h"
#include "ObjModel.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
ObjModel* model;
ObjModel* circuit;

void init();
void update();
void draw();

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
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (key == GLFW_KEY_ESCAPE)
                glfwSetWindowShouldClose(window, true);
        });

    camera = new FpsCam(window);

    tigl::shader->enableLighting(true);
    tigl::shader->setLightCount(1);
    tigl::shader->setLightDirectional(0, true);
    tigl::shader->setLightPosition(0, glm::normalize(glm::vec3(1, 1, 1)));
    tigl::shader->setLightAmbient(0, glm::vec3(0.5f, 0.5f, 0.5f));
    tigl::shader->setLightDiffuse(0, glm::vec3(0.5f, 0.5f, 0.5f));
    tigl::shader->setLightSpecular(0, glm::vec3(1, 1, 1));
    tigl::shader->setShinyness(0);

    tigl::shader->enableTexture(true);

    model = new ObjModel("models/car/temp/Dababy Convirtible.obj");
    circuit = new ObjModel("models/circuit/circuit.obj");
    
}


void update()
{
    camera->update(window);
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPointSize(10.0f);
    model->draw();
	circuit->draw();
}