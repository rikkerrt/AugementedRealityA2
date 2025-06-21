#include "MapLoader.h"
#include "GameObject.h"
#include "ModelComponent.h"
#include "SceneObject.h"
#include "PhysicsComponent.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <random>


void loadMap(SceneObject* scene)
{
    auto groundLayer = std::make_shared<GameObject>();
    groundLayer->position = glm::vec3(40, -0.2, 0);
    groundLayer->addComponent(std::make_shared<ModelComponent>("models/test/GroundLayer/GroundLayer.obj"));
    scene->addGameObject(groundLayer);

	buildTrackFromFile("models/Circuit/Circuitvolgorde.txt", scene);


    for (int i = 0; i < 25; ++i) {
        bool OnRoad = false;
        bool running = true;
        float randomFloatX, randomFloatZ;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(-200.0f, 200.0f);
        std::uniform_int_distribution<int> distInt(1, 3);

        while (running) {
            randomFloatX = dist(gen);
            randomFloatZ = dist(gen);
            std::cout << randomFloatX << randomFloatZ << std::endl;

            std::list<RoadComponent::BoundingBox*> roadBoxes = scene->getRoadBoxes();
            for (auto& boundingBox : roadBoxes) {
                if (-randomFloatX < boundingBox->tl.x && -randomFloatX > boundingBox->br.x &&
                    -randomFloatZ < boundingBox->tl.y && -randomFloatZ > boundingBox->br.y) {
                    OnRoad = true;
                    std::cout << "Inside for loop" << i << std::endl;
                    break;
                }
            }
            if (!OnRoad)
            {
                running = false;
                std::cout << "Outside if statement" << i << std::endl;
            }
            OnRoad = false;
        }
        auto prop = std::make_shared<GameObject>();
        prop->position = glm::vec3(randomFloatX, 0, randomFloatZ);
        prop->addComponent(std::make_shared<ModelComponent>("models/test/props/Tree" + std::to_string(distInt(gen)) + ".obj"));
		prop->addComponent(std::make_shared<PhysicsComponent>(3.0f));
        scene->addGameObject(prop);
    }


}

void buildTrackFromFile(const std::string& filename, SceneObject* scene)
{
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cout << "Kon baanbestand niet openen: " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(infile, line);
    infile.close();

    std::stringstream ss(line);
    std::string token;

    while (std::getline(ss, token, ',')) {
        int segmentType = std::stoi(token);
        auto segment = std::make_shared<GameObject>();
        segment->position = glm::vec3(0, 0, 0);

        if (segmentType == 1) {
            segment->addComponent(std::make_shared<ModelComponent>("models/test/straight/curve.obj"));
            scene->addRoadObject(segment, 1);
        }
        else if (segmentType == 2) {
            segment->addComponent(std::make_shared<ModelComponent>("models/test/corner/CurveRight.obj"));
            scene->addRoadObject(segment, 2);
        }
        else if (segmentType == 3) {
            segment->addComponent(std::make_shared<ModelComponent>("models/test/corner/CurveLeft.obj"));
            scene->addRoadObject(segment, 3);
        }
        else if (segmentType == 4) {
            segment->addComponent(std::make_shared<ModelComponent>("models/test/timing/StartFinish.obj"));
            scene->addRoadObject(segment, 4);
        }
        else if (segmentType == 5) {
            segment->addComponent(std::make_shared<ModelComponent>("models/test/timing/SectorStraight.obj"));
            scene->addRoadObject(segment, 5);
        }
    }
}
