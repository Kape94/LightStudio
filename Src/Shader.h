#ifndef _SHADER_H
#define _SHADER_H

#include <glm/vec3.hpp>

class Shader {
  public:

    void Create(
      const char* vertexShaderCode, 
      const char* fragmentShaderCode
    );

    void Delete();

    void Use();

    void SetUniform(
      const char* uniformName, 
      const glm::vec3& value
    ) const;

    void SetUniform(
      const char* uniformName, 
      const float value
    ) const;

    void SetUniform(
      const char* uniformName,
      const int value
    ) const;

  private:

    unsigned id = 0;
};

#endif
