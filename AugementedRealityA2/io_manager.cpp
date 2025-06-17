#include <iostream>
#include <fstream>
#include <vector>
#include "race_data.h"

std::string getCurrentDate();
int getLastId(const std::string& filename);

std::vector<RaceData> readFile(const std::string& filename) {
    std::vector<RaceData> raceList;
    std::ifstream infile(filename);

    if (infile.is_open()) {
        RaceData race;
        while (infile >> race.id >> race.date >> race.racetime) {
            raceList.push_back(race);
        }
        infile.close();
    }
    else {
        std::cerr << "Kan bestand niet openen: " << filename << std::endl;
    }

    return raceList;
}

void writeFile(const std::string& filename, double lapTime) {
    std::ofstream outfile(filename, std::ios::app);

    if (outfile.is_open()) {

        outfile << (getLastId("test.txt")) + 1 << " " << getCurrentDate() << " " << lapTime << std::endl;

        outfile.close();
    }
    else {
        std::cerr << "Kan bestand niet openen: " << filename << std::endl;
    }
}

int getLastId(const std::string& filename) {
    std::ifstream infile(filename);
    int lastId = 0;

    if (infile.is_open()) {
        RaceData race;
        while (infile >> race.id >> race.date >> race.racetime) {
            lastId = race.id;
        }
        infile.close();
    }
    else {
        std::cerr << "Kan bestand niet openen: " << filename << std::endl;
    }

    return lastId;
}

std::string getCurrentDate() {
    time_t now = time(0);
    struct tm localtm;

    localtime_s(&localtm, &now);

    char buffer[11];
    strftime(buffer, sizeof(buffer), "%d-%m-%Y", &localtm);

    return std::string(buffer);
}