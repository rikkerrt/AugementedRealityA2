#include <iostream>
#include <fstream>
#include <vector>
#include "race_data.h"

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

void writeFile(const std::string& filename, RaceData raceData) {
    std::ofstream outfile(filename, std::ios::app);

    if (outfile.is_open()) {
            outfile << raceData.id << " " << raceData.date << " " << raceData.racetime << std::endl;
        
        outfile.close();
    }
    else {
        std::cerr << "Kan bestand niet openen: " << filename << std::endl;
    }
}