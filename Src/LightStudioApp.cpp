#include "LightStudioApp.h"

#include "Buffer.h"
#include "Shader.h"

#include <vector>

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

const char* phongVertexShader = R"(
  #version 330

  layout (location = 0) in vec3 pos;
  layout (location = 1) in vec3 norm;

  out vec3 fragPos;
  out vec3 fragNorm;

  void main()
  {
    fragPos = pos;
    fragNorm = norm;
    gl_Position = vec4(pos, 1.0);
  }
)";

const char* phongFragmentShader = R"(
#version 330

struct DirectionalLight {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec3 direction;
};

struct PointLight {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  
  vec3 position;
  float constant;
  float linear;
  float quadratic;
};

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

#define MAX_LIGHTS 4

uniform Material material;

uniform DirectionalLight directionalLights[MAX_LIGHTS];
uniform int nDirectionalLights;

uniform PointLight pointLights[MAX_LIGHTS];
uniform int nPointLights;

uniform vec3 viewPosition;

in vec3 fragPos;
in vec3 fragNorm;

out vec4 fragColor;

vec3 CalculateDirectionalLight();
vec3 CalculatePointLight();

void main()
{
    vec3 directional = CalculateDirectionalLight();
    vec3 point = CalculatePointLight();
    
    vec3 result = directional + point;
	  fragColor = vec4(result, 1.0);
}


vec3 CalculateDirectionalLight() {
    vec3 viewDir = normalize(viewPosition - fragPos);
    
    vec3 normal = normalize(fragNorm);

    vec3 result = vec3(0.0);
    int nLights = min(nDirectionalLights, MAX_LIGHTS);
    for (int i = 0; i < nLights; ++i) {
        DirectionalLight light = directionalLights[i];
        
	    vec3 lightDir = normalize(light.direction);
        vec3 invLightDir = -lightDir;
        
        float diff = max(dot(normal, invLightDir), 0.0);
        
        vec3 reflectDir = reflect(lightDir, normal);
        float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
        
        vec3 ambient = light.ambient * material.ambient;
        vec3 diffuse = light.diffuse * (diff * material.diffuse);
        vec3 specular = light.specular * (spec * material.specular);
        
        result += (ambient + diffuse + specular);
    }
    return result; 
}

vec3 CalculatePointLight() {
    vec3 viewDir = normalize(viewPosition - fragPos);
    vec3 normal = normalize(fragNorm);

    vec3 result = vec3(0.0);
    int nLights = min(nPointLights, MAX_LIGHTS);
    for (int i = 0; i < nLights; ++i) {
        PointLight light = pointLights[i];
        
	    vec3 lightDir = normalize(fragPos - light.position);
        vec3 invLightDir = -lightDir;
        
        float diff = max(dot(invLightDir, normal), 0.0);
        
        vec3 reflectDir = reflect(lightDir, normal);
        float spec = pow( max(dot(reflectDir, viewDir), 0.0) , material.shininess );
        
        float distance = length(light.position - fragPos);
        float factor = light.constant + light.linear * distance + light.quadratic * (distance * distance);
        float attenuation = 1.0 / factor;
        
        vec3 ambient = (light.ambient * material.ambient) * attenuation;
        vec3 diffuse = ( light.diffuse * (diff * material.diffuse) ) * attenuation;
        vec3 specular = ( light.specular * (spec * material.specular) ) * attenuation;
        
        result += (ambient + diffuse + specular);
        
    }
    return result;
}
)";

Shader shader;
Buffer buffer;

//-----------------------------------------------------------------------------

void LightStudioApp::Initialize()
{
  shader.Create(phongVertexShader, phongFragmentShader);
  
  std::vector<float> vertices = {
    0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
    0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
    0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
    0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f
  };
  const unsigned nFloats = vertices.size();

  std::vector<unsigned> indices = { 0, 1, 2, 1, 3, 2 };
  const unsigned nIndices = indices.size();

  buffer.Create(
    vertices.data(),
    nFloats,
    indices.data(),
    nIndices,
    { {3, 0}/*position*/, {3, 1}/*normal*/}
  );
}

//-----------------------------------------------------------------------------

void LightStudioApp::Render()
{
  shader.SetUniform("material.ambient", glm::vec3(1.0f, 0.0f, 1.0f));
  shader.SetUniform("material.diffuse", glm::vec3(1.0f, 0.0f, 1.0f));
  shader.SetUniform("material.specular", glm::vec3(1.0f, 0.0f, 1.0f));
  shader.SetUniform("material.shininess", 32.0f);

  shader.SetUniform("directionalLights[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
  shader.SetUniform("directionalLights[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
  shader.SetUniform("directionalLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
  shader.SetUniform("directionalLights[0].direction", glm::vec3(0.0f, 0.0f, 1.0f));

  shader.SetUniform("nDirectionalLights", 1);
  shader.SetUniform("nPointLights", 0);

  shader.SetUniform("viewPosition", glm::vec3(0.0f, 0.0f, -1.0f));

  shader.Use();
  buffer.Render();
}

//-----------------------------------------------------------------------------
