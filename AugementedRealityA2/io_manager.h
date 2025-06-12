#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <string>
#include <vector>
#include "race_data.h"

std::vector<RaceData> readFile(const std::string& filename);
void writeFile(const std::string& filename, RaceData raceData);

#endif 