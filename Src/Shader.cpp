#include "Shader.h"

#include <gl/glew.h>

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
