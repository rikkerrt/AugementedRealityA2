#pragma once
#include "SceneObject.h"
#include <string>

void loadMap(SceneObject* scene);

void buildTrackFromFile(const std::string& filename, SceneObject* scene);

