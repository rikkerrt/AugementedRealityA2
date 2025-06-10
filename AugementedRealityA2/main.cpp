#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include "FpsCam.h"
#include "ObjModel.h"
#include "TextBox.h"

#include "GameObject.h"
#include "CubeComponent.h"
#include "ModelComponent.h"
#include "KeyboardSteeringComponent.h"
#include "VisionSteeringComponent.h"
#include "CarPhysicsComponent.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
ObjModel* model;
FpsCam* camera;
TextBox* textBox;
ObjModel* circuit;

void init();
void update();
void draw();

int main(void)
{
    if (!glfwInit())
        throw "Could not initialize glfw";

    window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glfw";
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
std::list<std::shared_ptr<GameObject>> objects;
std::shared_ptr<GameObject> player;


void init()
{
    int value[10];
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, value);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (key == GLFW_KEY_ESCAPE)
                glfwSetWindowShouldClose(window, true);
        });

    camera = new FpsCam(window, glm::vec3(0, -1, -1));

    player = std::make_shared<GameObject>();
    player->position = glm::vec3(0, 0, 5);
    player->addComponent(std::make_shared<ModelComponent>("models/car/carNoWindow.obj"));

    //// Keyboard steering wheel
    player->addComponent(std::make_shared<KeyboardSteeringComponent>());

    // Vision steering wheel
	/*VideoCapture webCam(0);
    VisionCalibration cal;

    /// CALIBRATION ///

    cal.addColor("Yellow");
    cal.addColor("Blue");
    
    cal.capurePhoto(webCam);
    cal.calibrate();*/
    //cal.save("calibration_settings.json");

    /// LOADING IN ///
    //cal.load("calibration_settings.json");

    //player->addComponent(std::make_shared<VisionSteeringComponent>(webCam, cal.getColors()));

    //auto baseComponent = player->getComponent<Component>();
    //auto visionComponent = std::dynamic_pointer_cast<VisionSteeringComponent>(baseComponent);

    //if (visionComponent) {
    //    visionComponent->setDebugMode(true);
    //    visionComponent->setMinimalMarkerSize(500);
    //}


    // Add more components

	player->addComponent(std::make_shared<CarPhysicsComponent>());
    objects.push_back(player);

    tigl::shader->enableTexture(true);

	auto circuit = std::make_shared<GameObject>();
	circuit->position = glm::vec3(0, 0, 0);
	circuit->addComponent(std::make_shared<ModelComponent>("models/circuit/circuit.obj"));
    objects.push_back(circuit);
}

    textBox = new TextBox("Hello", glm::vec2(1500, 50), glm::vec2(300, 80));
    textBox->loadFont("fonts/Opensans.ttf");
}

void update()
{
    camera->update(window, player->position, player->rotation);
	for (auto& o : objects)
		o->update(0.01f);

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

    // Text overlay
    tigl::shader->setViewMatrix(glm::mat4(1.0f));
    glm::mat4 orthoProjection = glm::ortho(0.0f, (float)viewport[2], (float)viewport[3], 0.0f);
    tigl::shader->setProjectionMatrix(orthoProjection);

    textBox->draw();

    for (auto& o : objects)
        o->draw();
}


