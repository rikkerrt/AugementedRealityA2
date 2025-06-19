#include "CheckPointManager.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "io_manager.h"

void CheckPointManager::init(std::vector<Zone>& checkPointZones, std::string fileName, int maxLaps)
{
    zones = checkPointZones;
	this->fileName = fileName;
	this->maxLaps = maxLaps;

    int index = 0;
    for (auto& zone : zones)
    {
        if (zone.type == ZoneType::Checkpoint) 
        {
            zone.index = index++;
            std::cout << "Zone " << std::endl;
        }
    }

    checkpointsCrossed = std::vector<bool>(index, false);
    startTime = std::chrono::steady_clock::now();
}

bool CheckPointManager::update(const glm::vec3& position, std::shared_ptr<TextBox> messageBox,
    std::shared_ptr<TextBox> timeBox, std::shared_ptr<TextBox> endBox,
    GLFWwindow* window)
{
    if (endGame) {
        endBox->setText("Game Over! Press Z to start again.\nFastest lap: " + std::to_string(fastestLap));
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            reset();
            endBox->setText("");
        }
        return false;
    }

    if (timing) {
        elapsedTime = std::chrono::steady_clock::now() - startTime;
        std::ostringstream stream;
        stream << std::fixed << std::setprecision(3) << elapsedTime.count();

        std::string sectorStatus;
        for (size_t i = 0; i < checkpointsCrossed.size(); ++i) {
            sectorStatus += "Sector " + std::to_string(i + 1) + ": " +
                (checkpointsCrossed[i] ? "V" : "X") + "  ";
        }

        timeBox->setText("Time: " + stream.str() + " sec\n" + sectorStatus);
    }


    for (const auto& zone : zones) {
        if (position.x >= zone.min.x && position.x <= zone.max.x &&
            position.z >= zone.min.z && position.z <= zone.max.z) {
            if (zone.type == ZoneType::Start) {
                handleStartZone(messageBox, endBox);
            }
            else if (zone.type == ZoneType::Checkpoint && zone.index >= 0 &&
                zone.index < checkpointsCrossed.size() && !checkpointsCrossed[zone.index]) {
                handleCheckpointZone(zone.index, messageBox);
            }
        }
    }
    return true;
}

void CheckPointManager::handleStartZone(std::shared_ptr<TextBox> messageBox, std::shared_ptr<TextBox> endBox)
{
    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - startTime).count() >= updateCoolDown) {
        messageBox->setText("You are at the start line!");
        startTime = std::chrono::steady_clock::now();
        timing = true;

        bool allPassed = std::all_of(checkpointsCrossed.begin(), checkpointsCrossed.end(), [](bool b) { return b; });
        if (allPassed) {
            completedLaps++;
            messageBox->setText("You completed lap " + std::to_string(completedLaps));
            lapTimes.push_back(elapsedTime);
        }

        if (completedLaps == maxLaps) {
            timing = false;
            endGame = true;
            completedLaps = 0;
            fastestLap = (*std::min_element(lapTimes.begin(), lapTimes.end())).count();
            writeFile(fileName, fastestLap);
            lapTimes.clear();
        }

        std::fill(checkpointsCrossed.begin(), checkpointsCrossed.end(), false);
    }
}

void CheckPointManager::handleCheckpointZone(int index, std::shared_ptr<TextBox> messageBox)
{
    checkpointsCrossed[index] = true;
    messageBox->setText("Checkpoint " + std::to_string(index + 1) + " reached!");
}

void CheckPointManager::reset()
{
    completedLaps = 0;
    endGame = false;
    std::fill(checkpointsCrossed.begin(), checkpointsCrossed.end(), false);
}

void CheckPointManager::handleEndGameInput(GLFWwindow* window, std::shared_ptr<TextBox> endBox)
{
    if (!endGame)
        return;

    endBox->setText("Game Over! Press Z to start again.\nFastest lap: " + std::to_string(fastestLap));

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        reset();
        endBox->setText("");
    }
}

