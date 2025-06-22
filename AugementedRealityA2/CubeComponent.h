#pragma once

#include "DrawComponent.h"
#include "tigl.h"

#include <vector>

using tigl::Vertex;

class CubeComponent : public DrawComponent
{
	std::vector<Vertex> verts;

public:
	CubeComponent(float size);
	~CubeComponent();


	virtual void draw() override;
};