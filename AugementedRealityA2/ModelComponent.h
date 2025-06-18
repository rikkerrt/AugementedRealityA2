#pragma once

#include "DrawComponent.h"
#include "tigl.h"
#include "ObjModel.h"
using tigl::Vertex;

#include <vector>

class ModelComponent : public DrawComponent
{
	ObjModel* model;

public:
	ModelComponent(const std::string& fileName);
	~ModelComponent();

	ObjModel* getModel();
	virtual void draw() override;
};