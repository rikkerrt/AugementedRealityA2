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
#include <chrono>
#include <iomanip>

using tigl::Vertex;

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

GLFWwindow* window;
FpsCam* camera;
TextBox* timeTextBox;
TextBox* textBox2;
TextBox* textBox3;
ObjModel* circuit;

int windowHeight;
int windowWidth;

glm::vec3 startLine1(-6, 0, -1);
glm::vec3 startLine2(6, 0, 1);
glm::vec3 checkPoint1l(59, 0, 84);
glm::vec3 checkPoint1r(72, 0, 86);
glm::vec3 checkPoint2l(95, 0, 0);
glm::vec3 checkPoint2r(107, 0, 2);

bool crossedCheckpoint1 = false;
bool crossedCheckpoint2 = false;
int completedLapsCount = 0;
int maxLaps = 3;

std::chrono::steady_clock::time_point startTime;
bool timing = false;

void init();
void update();
void draw();

int main(void)
{
    if (!glfwInit())
        throw "Could not initialize glwf";
    int count;
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

std::list<std::shared_ptr<GameObject>> objects;
std::shared_ptr<GameObject> player;


void init()
{
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    int value[10];
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, value);
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (key == GLFW_KEY_ESCAPE)
                glfwSetWindowShouldClose(window, true);
        });

    camera = new FpsCam(window, glm::vec3(0, -1, -10));

    player = std::make_shared<GameObject>();
    player->position = glm::vec3(0, 0, 20);
    player->addComponent(std::make_shared<ModelComponent>("models/car/carNoWindow.obj"));

    //// Keyboard steering wheel
    //player->addComponent(std::make_shared<KeyboardSteeringComponent>());

    // Vision steering wheel
	VideoCapture webCam(0);
    VisionCalibration cal;

    /// CALIBRATION ///

    /*cal.addColor("Yellow");
    cal.addColor("Blue");
    
    cal.capurePhoto(webCam);
    cal.calibrate();
    cal.save("calibration_settings.json");*/

    /// LOADING IN ///
    cal.load("calibration_settings.json");

    player->addComponent(std::make_shared<VisionSteeringComponent>(webCam, cal.getColors()));

    auto baseComponent = player->getComponent<Component>();
    auto visionComponent = std::dynamic_pointer_cast<VisionSteeringComponent>(baseComponent);

    if (visionComponent) {
        visionComponent->setDebugMode(true);
        visionComponent->setMinimalMarkerSize(50);
    }

    // Add more components

	player->addComponent(std::make_shared<CarPhysicsComponent>());
    objects.push_back(player);

    tigl::shader->enableTexture(true);

	auto circuit = std::make_shared<GameObject>();
	circuit->position = glm::vec3(0, 0, 0);
	circuit->addComponent(std::make_shared<ModelComponent>("models/circuit/circuit.obj"));
    objects.push_back(circuit);

    timeTextBox = new TextBox("Hello", glm::vec2(windowWidth - 300, 10), glm::vec2(300, 80));
	timeTextBox->loadFont("fonts/Opensans.ttf");

    textBox2 = new TextBox("Hello", glm::vec2(windowHeight - 300, 40), glm::vec2(300, 80));
    textBox2->loadFont("fonts/Opensans.ttf");

    textBox3 = new TextBox("Hello", glm::vec2(windowHeight -1000, 10), glm::vec2(300, 80));
    textBox3->loadFont("fonts/Opensans.ttf");
}

void update()
{
    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    camera->update(window, player->position, player->rotation);
    textBox3->setText(std::to_string(camera->position.x) + ", " + std::to_string(camera->position.z));

	if (camera->position.x >= startLine1.x && camera->position.x <= startLine2.x &&
        camera->position.z >= startLine1.z && camera->position.z <= startLine2.z)
	{
		textBox2->setText("You are at the start line!");
        startTime = std::chrono::steady_clock::now();
        timing = true;

        if (crossedCheckpoint1 && crossedCheckpoint2)
        {
            completedLapsCount++;
            textBox2->setText("You have completed " + std::to_string(completedLapsCount) + " laps!");
        }
        if (completedLapsCount == maxLaps)
        {
            //stop game
            timing = false;
            completedLapsCount = 0;
        }

        crossedCheckpoint1 = false;
        crossedCheckpoint2 = false;
	}
    if (timing)
    {
        std::chrono::duration<double> elapsedTime = std::chrono::steady_clock::now() - startTime;
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(3) << elapsedTime.count();
        timeTextBox->setText("Time elapsed: " + stream.str() + " seconds");
    }
	if (camera->position.x >= checkPoint1l.x && camera->position.x <= checkPoint1r.x &&
        camera->position.z >= checkPoint1l.z && camera->position.z <= checkPoint1r.z)
	{
		textBox2->setText("You are at checkpoint 1!");
        crossedCheckpoint1 = true;
	}
	if (camera->position.x >= checkPoint2l.x && camera->position.x <= checkPoint2r.x &&
        camera->position.z >= checkPoint2l.z && camera->position.z <= checkPoint2r.z)
	{
		textBox2->setText("You are at checkpoint 2!");
        crossedCheckpoint2 = true;
	}

    for (auto& o : objects)
    {
        o->update(0.01f);
    }

    float elapsedTime = static_cast<float>(currentTime - lastTime);
    lastTime = currentTime;

    camera->update(window, player->position, player->rotation);
    for (auto& o : objects)
        o->update(elapsedTime);
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

    for (auto& o : objects)
        o->draw();

    // Text overlay
    tigl::shader->setViewMatrix(glm::mat4(1.0f));
    glm::mat4 orthoProjection = glm::ortho(0.0f, (float)viewport[2], (float)viewport[3], 0.0f);
    tigl::shader->setProjectionMatrix(orthoProjection);

    timeTextBox->draw();
	textBox2->draw();
	textBox3->draw();
}


