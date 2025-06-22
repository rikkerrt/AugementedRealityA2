#include "CheckPointManager.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "io_manager.h"

void CheckPointManager::init(std::vector<CheckPoint>& checkPointZones, std::string fileName, int maxLaps)
{
    checkPoints = checkPointZones;
	this->fileName = fileName;
	this->maxLaps = maxLaps;

    int index = 0;
    for (auto& checkPoint : checkPoints)
    {
        if (checkPoint.type == ZoneType::Checkpoint) 
        {
            checkPoint.index = index++;
            std::cout << "Zone " << std::endl;
        }
    }

    checkpointsCrossed = std::vector<bool>(index, false);
    startTime = std::chrono::steady_clock::now();
}

bool CheckPointManager::update(const glm::vec3& position, std::shared_ptr<TextBox> messageBox,
    std::shared_ptr<TextBox> timeBox, GLFWwindow* window)
{
    if (endGame) {
       
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

    for (const auto& checkPoint : checkPoints) {
        if (position.x >= checkPoint.min.x && position.x <= checkPoint.max.x &&
            position.z >= checkPoint.min.z && position.z <= checkPoint.max.z) {

            lastCheckpoint = checkPoint;

            if (checkPoint.type == ZoneType::Start) {
                handleStartZone(messageBox);
            }
            else if (checkPoint.type == ZoneType::Checkpoint && checkPoint.index >= 0 &&
                checkPoint.index < checkpointsCrossed.size() && !checkpointsCrossed[checkPoint.index]) {
                handleCheckpointZone(checkPoint.index, messageBox);
            }
        }
    }

    return true;
}

void CheckPointManager::handleStartZone(std::shared_ptr<TextBox> messageBox)
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
    lapTimes.clear();
	lastCheckpoint = { glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), ZoneType::Start };
    completedLaps = 0;
    endGame = false;
    std::fill(checkpointsCrossed.begin(), checkpointsCrossed.end(), false);
}

CheckPoint CheckPointManager::getLastCheckpoint()
{
	return lastCheckpoint;
}

double CheckPointManager::getFastestLap()
{
	return fastestLap;
}
