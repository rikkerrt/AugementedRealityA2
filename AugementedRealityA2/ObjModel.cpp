#include "ObjModel.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

#include "tigl.h"
#include "Texture.h"

using tigl::Vertex;

/**
* Replaces a substring in a string
*/
static std::string replace(std::string str, const std::string &toReplace, const std::string &replacement)
{
	size_t index = 0;
	while (true) 
	{
		 index = str.find(toReplace, index);
		 if (index == std::string::npos) 
			 break;
		 str.replace(index, toReplace.length(), replacement);
		 ++index;
	}
	return str;
}

/**
* Splits a string into substrings, based on a seperator
*/
static std::vector<std::string> split(std::string str, const std::string &seperator)
{
	std::vector<std::string> ret;
	size_t index;
	while(true)
	{
		index = str.find(seperator);
		if(index == std::string::npos)
			break;
		ret.push_back(str.substr(0, index));
		str = str.substr(index+1);
	}
	ret.push_back(str);
	return ret;
}

/**
* Turns a string to lowercase
*/
static inline std::string toLower(std::string data)
{
	std::transform(data.begin(), data.end(), data.begin(), ::tolower);
	return data;
}


/**
* Cleans up a line for processing
*/
static inline std::string cleanLine(std::string line)
{
	line = replace(line, "\t", " ");
	while (line.find("  ") != std::string::npos)
		line = replace(line, "  ", " ");
	if (line == "")
		return "";
	if (line[0] == ' ')
		line = line.substr(1);
	if (line == "")
		return "";
	if (line[line.length() - 1] == ' ')
		line = line.substr(0, line.length() - 1);
	return line;
}




/**
* Loads an object model
*/
ObjModel::ObjModel(const std::string &fileName)
{
	std::cout << "Loading " << fileName << std::endl;
	std::string dirName = fileName;
	if(dirName.rfind("/") != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind("/"));
	if(dirName.rfind("\\") != std::string::npos)
		dirName = dirName.substr(0, dirName.rfind("\\"));
	if(fileName == dirName)
		dirName = "";


	std::ifstream pFile(fileName.c_str());

	if (!pFile.is_open())
	{
		std::cout << "Could not open file " << fileName << std::endl;
		return;
	}


	ObjGroup* currentGroup = new ObjGroup();
	currentGroup->materialIndex = -1;


	while(!pFile.eof())
	{
		std::string line;
		std::getline(pFile, line);
		line = cleanLine(line);
		if(line == "" || line[0] == '#') //skip empty or commented line
			continue;

		std::vector<std::string> params = split(line, " ");
		params[0] = toLower(params[0]);

		if(params[0] == "v")
			vertices.push_back(glm::vec3((float)atof(params[1].c_str()), (float)atof(params[2].c_str()), (float)atof(params[3].c_str())));
		else if(params[0] == "vn")
			normals.push_back(glm::vec3((float)atof(params[1].c_str()), (float)atof(params[2].c_str()), (float)atof(params[3].c_str())));
		else if(params[0] == "vt")
			texcoords.push_back(glm::vec2((float)atof(params[1].c_str()), (float)atof(params[2].c_str())));
		else if(params[0] == "f")
		{
			for(size_t ii = 4; ii <= params.size(); ii++)
			{
				Face face;

				for(size_t i = ii-3; i < ii; i++)	//magische forlus om van quads triangles te maken ;)
				{
					Vertex vertex;
					std::vector<std::string> indices = split(params[i == (ii-3) ? 1 : i], "/");
					if (indices.size() >= 1)	//er is een positie
						vertex.position = atoi(indices[0].c_str()) - 1;
					if(indices.size() == 2)		//alleen texture
						vertex.texcoord = atoi(indices[1].c_str())-1;
					if(indices.size() == 3)		//v/t/n of v//n
					{
						if( indices[1] != "")
							vertex.texcoord = atoi(indices[1].c_str())-1;
						vertex.normal = atoi(indices[2].c_str())-1;
					}
					face.vertices.push_back(vertex);
				}
				currentGroup->faces.push_back(face);
			}
		}
		else if(params[0] == "s")
		{//smoothing groups
		}
        else if(params[0] == "mtllib")
        {
            loadMaterialFile(dirName + "/" + params[1], dirName);
        }
		else if(params[0] == "usemtl")
		{
			if(currentGroup->faces.size() != 0)
				groups.push_back(currentGroup);
			currentGroup = new ObjGroup();
			currentGroup->materialIndex = -1;

			for(size_t i = 0; i < materials.size(); i++)
			{
				MaterialInfo* info = materials[i];
				if(info->name == params[1])
				{
					currentGroup->materialIndex = i;
					break;
				}
			}
			if(currentGroup->materialIndex == -1)
				std::cout<<"Could not find material name "<<params[1]<<std::endl;
		}
	}
	groups.push_back(currentGroup);
}


ObjModel::~ObjModel(void)
{
}




void ObjModel::draw() {
	for (ObjGroup* group : groups) {
		if (group->materialIndex >= 0 && group->materialIndex < (int)materials.size()) {
			MaterialInfo* mat = materials[group->materialIndex];
			
			if (mat->texture) {
				mat->texture->bind();
				tigl::shader->setColorMult(glm::vec4(1, 1, 1, mat->transparency));
			}

			
			else {
				tigl::shader->setColorMult(glm::vec4(1, 1, 1, 0.5));
			}
		}
		
		for (auto& face : group->faces) {
			std::vector<tigl::Vertex> verts;
			for (auto& v : face.vertices) {
				glm::vec3 pos(0.0f), norm(0.0f);
				glm::vec2 tex(0.0f);
				if (v.position >= 0 && v.position < (int)vertices.size()) {
					pos = vertices[v.position];
				}
				if (v.normal >= 0 && v.normal < (int)normals.size()) {
					norm = normals[v.normal];
				}
				if (v.texcoord >= 0 && v.texcoord < (int)texcoords.size()) {
					tex = glm::vec2(texcoords[v.texcoord].x, 1.0f - texcoords[v.texcoord].y);
				}
				verts.push_back(tigl::Vertex::PTN(pos, tex, norm));
				//cachen
			}
			tigl::drawVertices(GL_TRIANGLES, verts);
		}
	}
}

void ObjModel::loadMaterialFile( const std::string &fileName, const std::string &dirName )
{
	std::cout << "Loading " << fileName << std::endl;
	std::ifstream pFile(fileName.c_str());
	if (!pFile.is_open())
	{
		std::cout << "Could not open file " << fileName << std::endl;
		return;
	}

	MaterialInfo* currentMaterial = NULL;

	while(!pFile.eof())
	{
		std::string line;
		std::getline(pFile, line);
		line = cleanLine(line);
		if(line == "" || line[0] == '#')
			continue;

		std::vector<std::string> params = split(line, " ");
		params[0] = toLower(params[0]);

		if(params[0] == "newmtl")
		{
			if(currentMaterial != NULL)
			{
				materials.push_back(currentMaterial);
			}

			currentMaterial = new MaterialInfo();
			currentMaterial->name = params[1];
		}
		else if(params[0] == "map_kd")
		{
			std::string tex = params[1];
			if (tex.find("/"))
				tex = tex.substr(tex.rfind("/") + 1);
			if (tex.find("\\"))
				tex = tex.substr(tex.rfind("\\") + 1);
			//TODO
			currentMaterial->texture = new Texture(dirName + "/" + tex);
		} 
		else if (params[0] == "kd")
		{//TODO, diffuse color
		}
		else if (params[0] == "ka")
		{//TODO, ambient color
		}
		else if (params[0] == "ks")
		{//TODO, specular color
		}
		else if (params[0] == "d")
		{
			currentMaterial->transparency = (float)atof(params[1].c_str());
			
			if (currentMaterial->transparency != 1) {
				std::cout << currentMaterial->transparency << std::endl;
			}

		}
		else if (
			params[0] == "illum" || 
			params[0] == "map_bump" || 
			params[0] == "map_ke" || 
			params[0] == "map_ka" ||
			params[0] == "map_d" ||
			params[0] == "ke" ||
			params[0] == "ns" ||
			params[0] == "ni" ||
			params[0] == "td" ||
			params[0] == "tf" ||
			params[0] == "tr" ||
			false)
		{
			//these values are usually not used for rendering at this time, so ignore them
		}
		else
			std::cout<<" Didn't parse "<<params[0]<<" in material file"<<std::endl;
	}

	if(currentMaterial != NULL)
		materials.push_back(currentMaterial);

}

ObjModel::MaterialInfo::MaterialInfo()
{
	texture = NULL;
}


