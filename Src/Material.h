#ifndef _MATERIAL_H
#define _MATERIAL_H

#include <glm/vec3.hpp>

struct Material {  
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;
};

#endif
