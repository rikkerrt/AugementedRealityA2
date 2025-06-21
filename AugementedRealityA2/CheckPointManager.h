#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <chrono>
#include <GLFW/glfw3.h>

#include "TextBox.h"

enum class ZoneType {
    Start,
    Checkpoint
};

struct CheckPoint {
    glm::vec3 min;
    glm::vec3 max;
    ZoneType type;
    int index = -1;
};

class CheckPointManager {
public:
    void init(std::vector<CheckPoint>& zones, std::string filename, int maxLaps);
    bool update(const glm::vec3& position, std::shared_ptr<TextBox> messageBox,
        std::shared_ptr<TextBox> timeBox, std::shared_ptr<TextBox> endBox,
        GLFWwindow* window);
    void reset();
    void handleEndGameInput(GLFWwindow* window, std::shared_ptr<TextBox> endBox);
    glm::vec3 getLastCheckpointPosition();

private:
    std::vector<CheckPoint> checkPoints;
    std::vector<bool> checkpointsCrossed;
    int completedLaps = 0;
    int maxLaps = 3;
    int updateCoolDown = 1;
    bool timing = false;
    bool endGame = false;
	std::string fileName;
    glm::vec3 lastCheckpointPosition = glm::vec3(0, 0, 0);

    std::chrono::steady_clock::time_point startTime;
    std::chrono::duration<double> elapsedTime;
    std::vector<std::chrono::duration<double>> lapTimes;
    double fastestLap = 0.0;

    void handleStartZone(std::shared_ptr<TextBox> messageBox, std::shared_ptr<TextBox> endBox);
    void handleCheckpointZone(int index, std::shared_ptr<TextBox> messageBox);
};
