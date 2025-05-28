#include "ModelComponent.h"

ModelComponent::ModelComponent(const std::string& fileName)
{
	model = new ObjModel(fileName);
}

ModelComponent::~ModelComponent()
{
}

void ModelComponent::draw()
{
	model->draw();
}