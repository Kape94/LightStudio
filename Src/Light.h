#ifndef _LIGHT_H
#define _LIGHT_H

#include <glm/vec3.hpp>

struct Light {
  bool enabled;
  bool isDirectional;
  
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;

  // directional
  glm::vec3 direction;

  // point
  glm::vec3 position;
  float quadratic;
  float linear;
  float constant;
};

#endif
