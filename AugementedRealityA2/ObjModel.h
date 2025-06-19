#pragma once  

#include <GL/glew.h>  
#include <glm/glm.hpp>  
#include "tigl.h"  
#include <string>  
#include <vector>  
#include <list>  

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

   std::vector<glm::vec3> vertices;  
   std::vector<glm::vec3> normals;  
   std::vector<glm::vec2> texcoords;  
   std::vector<ObjGroup*> groups;  
   std::vector<MaterialInfo*> materials;  
   RoadComponent::Rect* rect; // Use fully qualified name  

   void buildBoundingBox();  
   void loadMaterialFile(const std::string &fileName, const std::string &dirName);  

   /// <summary>  
   /// Build a vertex buffer object from the model  
   /// </summary>  
   void buildVertexBuffer();  

public:  
   ObjModel(const std::string &filename);  
   ~ObjModel(void);  

   void draw();  
   RoadComponent::Rect* getRect(); // Use fully qualified name  
};
