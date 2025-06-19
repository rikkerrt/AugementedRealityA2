#include <iostream>
#include <list>
#include <chrono>
#include <iomanip>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")

#include "tigl.h"
#include "ObjModel.h"
#include "io_manager.h"

#include "GameObject.h"
#include "CameraManager.h"
#include "ModelComponent.h"
#include "PerspectiveCameraComponent.h"
#include "KeyboardSteeringComponent.h"
#include "VisionSteeringComponent.h"
#include "CarPhysicsComponent.h"
#include "ChildComponent.h"

#include "TextBox.h"

using tigl::Vertex;


GLFWwindow* window;
CameraManager* cameraManager;

/* Dit blok hier moet echt weg*/
TextBox* textBox;
ObjModel* circuit;

int windowHeight;
int windowWidth;

TextBox* timeTextBox;
TextBox* textBox2;
TextBox* textBox3;

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

string fileName = "TimeFile.txt";

std::chrono::steady_clock::time_point startTime;
std::chrono::duration<double> elapsedTime;
std::vector<std::chrono::duration<double>> lapTimes;

bool timing = false;

void init();
void update();
void draw();

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
std::shared_ptr<GameObject> car;

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

    // Add car
    car = std::make_shared<GameObject>();
    car->position = glm::vec3(0, 0, 5);
    car->addComponent(std::make_shared<ModelComponent>("models/car/carNoSteeringWheel.obj"));
    car->addComponent(std::make_shared<CarPhysicsComponent>(15));
    objects.push_back(car);

    // Add Steering wheel
    auto steeringWheel = std::make_shared<GameObject>();
    steeringWheel->addComponent(std::make_shared<ModelComponent>("models/car/steeringWheel.obj"));
    steeringWheel->addComponent(std::make_shared<ChildComponent>(car));
    objects.push_back(steeringWheel);

    // Add circuit
    auto circuit = std::make_shared<GameObject>();
    circuit->position = glm::vec3(0, 0, 0);
    circuit->addComponent(std::make_shared<ModelComponent>("models/circuit/circuit.obj"));
    objects.push_back(circuit);

    textBox = new TextBox("Hello", glm::vec2(1500, 50), glm::vec2(300, 80));
    textBox->loadFont("fonts/Opensans.ttf");

    timeTextBox = new TextBox("TimeTextBox", glm::vec2(windowWidth - 300, 10), glm::vec2(300, 80));
    textBox->loadFont("fonts/Opensans.ttf");

    textBox2 = new TextBox("Hello", glm::vec2(windowHeight - 300, 50), glm::vec2(300, 80));
    textBox2->loadFont("fonts/Opensans.ttf");
    // Add Camera
    auto camera = std::make_shared<GameObject>();
    camera->addComponent(std::make_shared<ChildComponent>(car, glm::vec3{0.3, 1.25, 0}));
    camera->addComponent(std::make_shared<PerspectiveCameraComponent>());
    objects.push_back(camera);
    cameraManager = new CameraManager(camera);

    // Pick between vision and keyboard steering
    bool useVision = false;
    bool calibrateVision = false;

    if (useVision)
    {
        VideoCapture webCam(0);
        VisionCalibration cal;

        if (calibrateVision)
        {
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
    glPointSize(10.0f);
    tigl::shader->enableColor(true);
    tigl::shader->enableTexture(true);
}

void update()
{
    //TODo deze logica moet hier echt uit
    //static double lastTime = glfwGetTime();
    //double currentTime = glfwGetTime();
    //camera->update(window, car->position, car->rotation);
    //textBox3->setText(std::to_string(car->position.x) + ", " + std::to_string(car->position.z));

    //if mag alleen 1 keer per seconden getriggerd worden
	if (car->position.x >= startLine1.x && car->position.x <= startLine2.x &&
        car->position.z >= startLine1.z && car->position.z <= startLine2.z &&
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - startTime).count() >= 1)
	{
		textBox2->setText("You are at the start line!");
        startTime = std::chrono::steady_clock::now();
        timing = true;

        if (crossedCheckpoint1 && crossedCheckpoint2)
        {
            completedLapsCount++;
            textBox2->setText("You have completed " + std::to_string(completedLapsCount) + " laps!");
			lapTimes.push_back(elapsedTime);
        }
        if (completedLapsCount == maxLaps)
        {
            //stop game
            timing = false;
            completedLapsCount = 0;

            double maxLapTime = (*std::max_element(lapTimes.begin(), lapTimes.end())).count();
			writeFile(fileName, maxLapTime);          
        }

        crossedCheckpoint1 = false;
        crossedCheckpoint2 = false;
	}
    if (timing)
    {
        elapsedTime = std::chrono::steady_clock::now() - startTime;
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(3) << elapsedTime.count();
        timeTextBox->setText("Time elapsed: " + stream.str() + " seconds");
    }
	if (car->position.x >= checkPoint1l.x && car->position.x <= checkPoint1r.x &&
        car->position.z >= checkPoint1l.z && car->position.z <= checkPoint1r.z)
	{
		textBox2->setText("You are at checkpoint 1!");
        crossedCheckpoint1 = true;
	}
	if (car->position.x >= checkPoint2l.x && car->position.x <= checkPoint2r.x &&
        car->position.z >= checkPoint2l.z && car->position.z <= checkPoint2r.z)
	{
		textBox2->setText("You are at checkpoint 2!");
        crossedCheckpoint2 = true;
	}

    // Deze logica niet!
    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float elapsedTime = static_cast<float>(currentTime - lastTime);
    lastTime = currentTime;

    for (auto& o : objects)
        o->update(elapsedTime);
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
    
    for (auto& o : objects)
        o->draw();

    // Dit zou ook nog wel ergens anders kunnen
    // Draw text/UI
    tigl::shader->setViewMatrix(glm::mat4(1.0f));
    glm::mat4 orthoProjection = glm::ortho(0.0f, (float)viewport[2], (float)viewport[3], 0.0f);
    tigl::shader->setProjectionMatrix(orthoProjection);

    timeTextBox->draw();
	textBox2->draw();
	//textBox3->draw();
}
