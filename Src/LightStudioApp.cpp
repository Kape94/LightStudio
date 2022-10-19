#include "LightStudioApp.h"

#include "Buffer.h"
#include "Shader.h"

const char* vShaderCode = R"(
  #version 330

  layout (location = 0) in vec3 pos;
  layout (location = 1) in vec3 col;

  out vec3 color;

  void main()
  {
    gl_Position = vec4(pos, 1.0);
    color = col;
  }
)";

const char* fShaderCode = R"(
  #version 330

  in vec3 color;

  out vec4 FragColor;

  void main() 
  {
    FragColor = vec4(color, 1.0);
  }
)";

Shader shader;
Buffer buffer;

//-----------------------------------------------------------------------------

void LightStudioApp::Initialize()
{
  shader.Create(vShaderCode, fShaderCode);
  
  float data[] = {
    0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f
  };
  constexpr unsigned nFloats = 24;

  unsigned indices[] = { 0, 1, 2, 1, 3, 2 };
  constexpr unsigned nIndices = 6;

  buffer.Create(
    data, 
    nFloats,
    indices,
    nIndices,
    { {3, 0}/*position*/, {3, 1}/*color*/}
  );
}

//-----------------------------------------------------------------------------

void LightStudioApp::Render()
{
  shader.Use();
  buffer.Render();
}

//-----------------------------------------------------------------------------
