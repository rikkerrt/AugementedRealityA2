#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "tigl.h"
#include <string>
#include <vector>
#include <list>
#include "tigl.h"

#include "RoadComponent.h"

class Texture;



class ObjModel
{
private:
	class Vertex
	{
	public:
		int position;
		int normal;
		int texcoord; 
	};

	class Face
	{
	public:
		std::list<Vertex> vertices;
	};
	class MaterialInfo
	{
	public:
		MaterialInfo();
		std::string name;
		Texture* texture;
		float transparency;
	};

	class ObjGroup
	{
	public:
		std::string name;
		int materialIndex;
		std::list<Face> faces;
		//cache
	};

	std::vector<tigl::Vertex> vboVertices;
	tigl::VBO* vbo = nullptr;

	glm::vec3 minimalBounds;
	glm::vec3 maximumBounds;

	std::vector<glm::vec3>	vertices;
	std::vector<glm::vec3>	normals;
	std::vector<glm::vec2>	texcoords;
	std::vector<ObjGroup*> groups;
	std::vector<MaterialInfo*> materials;
	Rect* rect;

	void buildBoundingBox();
	void loadMaterialFile(const std::string &fileName, const std::string &dirName);

	/// <summary>
	/// Build a vertex buffer object from the moadel
	/// </summary>
	void buildVertexBuffer();

	/// <summary>
	/// Construct the minimal bounds vertex
	/// </summary>
	void buildMinimalBounds();

	/// <summary>
	/// Construct the maximum bound vertex
	/// </summary>
	void buildMaximumBound();
public:
	ObjModel(const std::string &filename);
	~ObjModel(void);

	void draw();
	Rect* getRect();
};

