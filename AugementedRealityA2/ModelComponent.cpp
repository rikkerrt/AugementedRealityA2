#include "ModelComponent.h"

ModelComponent::ModelComponent(const std::string& fileName)
{
	model = new ObjModel(fileName);
}

ModelComponent::~ModelComponent()
{
}

ObjModel* ModelComponent::getModel()
{
	return model;
}

void ModelComponent::draw()
{
	model->draw();
}