#include "Shader.h"

#include <GL/glew.h>

#include <glm/gtc/type_ptr.hpp>

//-----------------------------------------------------------------------------

void Shader::Create(
  const char* vertexShaderCode, 
  const char* fragmentShaderCode
)
{
  const unsigned vShaderID = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShaderID, 1, &vertexShaderCode, nullptr);
  glCompileShader(vShaderID);

  const unsigned fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShaderID, 1, &fragmentShaderCode, nullptr);
  glCompileShader(fShaderID);

  const unsigned programID = glCreateProgram();
  glAttachShader(programID, vShaderID);
  glAttachShader(programID, fShaderID);
  glLinkProgram(programID);

  glDeleteShader(vShaderID);
  glDeleteShader(fShaderID);

  this->id = programID;
}

//-----------------------------------------------------------------------------

void Shader::Delete()
{
  glDeleteProgram(this->id);
}

//-----------------------------------------------------------------------------

void Shader::Use()
{
  glUseProgram(this->id);
}

//-----------------------------------------------------------------------------

void Shader::SetUniform(
  const char* uniformName,
  const glm::mat4x4& value
) const
{
  const int loc = glGetUniformLocation(this->id, uniformName);
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

//-----------------------------------------------------------------------------

void Shader::SetUniform(
  const char* uniformName,
  const glm::vec3& value
) const
{
  const int loc = glGetUniformLocation(this->id, uniformName);
  glUniform3f(loc, value.x, value.y, value.z);
}

//-----------------------------------------------------------------------------

void Shader::SetUniform(
  const char* uniformName,
  const float value
) const
{
  const int loc = glGetUniformLocation(this->id, uniformName);
  glUniform1f(loc, value);
}

//-----------------------------------------------------------------------------

void Shader::SetUniform(
  const char* uniformName, 
  const int value
) const
{
  const int loc = glGetUniformLocation(this->id, uniformName);
  glUniform1i(loc, value);
}

//-----------------------------------------------------------------------------
