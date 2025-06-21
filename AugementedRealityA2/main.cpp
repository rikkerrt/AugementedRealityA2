#include <iostream>
#include <list>
#include <chrono>
#include <iomanip>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "tigl.h"
#include "ObjModel.h"
#include "io_manager.h"
#include "CheckPointManager.h"
#include "GameObject.h"
#include "SceneObject.h"
#include "CubeComponent.h"
#include "CameraManager.h"
#include "ModelComponent.h"
#include "PerspectiveCameraComponent.h"
#include "KeyboardSteeringComponent.h"
#include "VisionSteeringComponent.h"
#include "CarPhysicsComponent.h"
#include "PhysicsComponent.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "RoadComponent.h"
#include "MapLoader.h"
#include "ChildComponent.h"

#include "TextBox.h"
using tigl::Vertex;


GLFWwindow* window;
CameraManager* cameraManager;

/* Dit blok hier moet echt weg*/
SceneObject scene;
CheckPointManager checkPointManager;

std::list<std::shared_ptr<TextBox>> textBoxes;
std::shared_ptr<TextBox> timeTextBox;
std::shared_ptr<TextBox> messageTextBox;
std::shared_ptr<TextBox> textBox3;
std::shared_ptr<TextBox> endGameTextBox;

std::shared_ptr<GameObject> car;


int windowHeight;
int windowWidth;

void initGame();
void update();
void draw();
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(void)
{
    if (!glfwInit())
        throw "Could not initialize glwf";
    int count;
    window = glfwCreateWindow(1280, 720, "Race Game", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw "Could not initialize glfw";
    }
    glfwMakeContextCurrent(window);

    tigl::init();
    initGame();
    

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

void initGame()
{
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    int value[10];
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, value);
    glfwSetKeyCallback(window, keyCallback);

    scene = SceneObject();

    // Add car
    car = std::make_shared<GameObject>();
    car->position = glm::vec3(0, 0, 5);
    car->addComponent(std::make_shared<ModelComponent>("models/car/carNoSteeringWheel.obj"));
    car->addComponent(std::make_shared<CarPhysicsComponent>(15));
    scene.addGameObject(car);

    // Add Steering wheel
    auto steeringWheel = std::make_shared<GameObject>();
    steeringWheel->addComponent(std::make_shared<ModelComponent>("models/car/steeringWheel.obj"));
    steeringWheel->addComponent(std::make_shared<ChildComponent>(car));
    scene.addGameObject(steeringWheel);

    // Add Camera
    auto camera = std::make_shared<GameObject>();
    camera->addComponent(std::make_shared<ChildComponent>(car, glm::vec3{0.3, 1.25, 0}));
    camera->addComponent(std::make_shared<PerspectiveCameraComponent>());
    scene.addGameObject(camera);

    loadMap(&scene);

    cameraManager = new CameraManager(camera);

    // Pick between vision and keyboard steering
    bool useVision = false;
    bool calibrateVision = false;

    if (useVision)
    {
        VideoCapture webCam(0);
        VisionCalibration cal;

        if (calibrateVision) {
            cal.addColor("Yellow");
            cal.addColor("Blue");

            cal.capurePhoto(webCam);
            cal.calibrate();
            cal.save("calibration_settings.json");
        }
        else
        {
            cal.load("calibration_settings.json");
        }

        car->addComponent(std::make_shared<VisionSteeringComponent>(webCam, cal.getColors()));

        auto baseComponent = car->getComponent<Component>();
        auto visionComponent = std::dynamic_pointer_cast<VisionSteeringComponent>(baseComponent);

        if (visionComponent) {
            visionComponent->setDebugMode(true);
            visionComponent->setMinimalMarkerSize(50);
        }
    }
    else
    {
        car->addComponent(std::make_shared<KeyboardSteeringComponent>());
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    tigl::shader->enableColor(true);
    tigl::shader->enableTexture(true);

    timeTextBox = std::make_shared<TextBox>("", glm::vec2(windowWidth * 0.95f - 300, windowHeight * 0.02f), "fonts/Opensans.ttf");
	textBoxes.push_back(timeTextBox);
    messageTextBox = std::make_shared<TextBox>("", glm::vec2(windowWidth * 0.15f, windowHeight * 0.88f - 20), "fonts/Opensans.ttf");
    textBoxes.push_back(messageTextBox);
    textBox3 = std::make_shared<TextBox>("", glm::vec2(windowWidth * 0.05f, windowHeight * 0.1f), "fonts/Opensans.ttf");
	textBoxes.push_back(textBox3);
    endGameTextBox = std::make_shared<TextBox>("", glm::vec2(windowWidth * 0.5f - 100, windowHeight * 0.5f), "fonts/Opensans.ttf");

    std::vector<CheckPoint> checkPoints;
	checkPoints = scene.getCheckPoints();
    checkPointManager.init(checkPoints, "TimeFile.txt", 3);
	for (auto& checkPoint : checkPoints)
	{
		std::cout << "CheckPoint: type " << (checkPoint.type == ZoneType::Start ? "Start" : "Checkpoint")
			<< ", index " << checkPoint.index << std::endl;
		std::cout << "CheckPoint: xmin " << checkPoint.min.x << ", zmin " << checkPoint.min.z << " - xmax " << checkPoint.max.x << ", zmax " << checkPoint.max.z << std::endl;
	}

    glPointSize(10.0f);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void update()
{
    bool gameContinues = checkPointManager.update(car->position, messageTextBox, timeTextBox, endGameTextBox, window);
    if (!gameContinues)
    {
        return;
    }

    // Deze logica niet!
    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float elapsedTime = static_cast<float>(currentTime - lastTime);
    lastTime = currentTime;

    textBox3->setText(std::to_string(car->position.x) + ", " + std::to_string(car->position.z));
    scene.update(elapsedTime);
}

void draw()
{
    glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float aspect = viewport[2] / (float)viewport[3];
    
    tigl::shader->setProjectionMatrix(cameraManager->getProjectionMatrix(aspect));
    tigl::shader->setViewMatrix(cameraManager->getViewMatrix());
    tigl::shader->setModelMatrix(glm::mat4(1.0f));
    
	scene.draw();

    // Dit zou ook nog wel ergens anders kunnen
    // Draw text/UI
    tigl::shader->setViewMatrix(glm::mat4(1.0f));
    glm::mat4 orthoProjection = glm::ortho(0.0f, (float)viewport[2], (float)viewport[3], 0.0f);
    tigl::shader->setProjectionMatrix(orthoProjection);

    for (auto& textBox : textBoxes)
    {
        textBox->draw();
    }
}
