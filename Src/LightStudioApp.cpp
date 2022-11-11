#include "LightStudioApp.h"

#include "Buffer.h"
#include "Shader.h"

#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

const char* vShaderCode = R"(
  #version 330

  layout (location = 0) in vec3 pos;
  layout (location = 1) in vec3 col;

  out vec3 color;

  uniform mat4 cameraTransform;

  void main()
  {
    gl_Position = cameraTransform * vec4(pos, 1.0);
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

  uniform mat4 cameraTransform;

  void main()
  {
    fragPos = pos;
    fragNorm = norm;
    gl_Position = cameraTransform * vec4(pos, 1.0);
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
    -1.0f, -1.0f, -1.0f, -0.57f, -0.57f, -0.57f,
    1.0f, -1.0f, -1.0f, 0.57f, -0.57f, -0.57f,
    1.0f, 1.0f, -1.0f, 0.57f, 0.57f, -0.57f,
    -1.0f, 1.0f, -1.0f, -0.57f, 0.57f, -0.57f,
    -1.0f, 1.0f, 1.0f, -0.57f, 0.57f, 0.57f,
    1.0f, 1.0f, 1.0f, 0.57f, 0.57f, 0.57f,
    1.0f, -1.0f, 1.0f, 0.57f, -0.57f, 0.57f,
    -1.0f, -1.0f, 1.0f, -0.57f, -0.57f, 0.57f
  };
  const unsigned nFloats = vertices.size();

  std::vector<unsigned> indices = {
    0, 1, 2, 0, 2, 3, // front
    6, 7, 4, 6, 4, 5, // back
    7, 0, 3, 7, 3, 4, // left
    1, 6, 5, 1, 5, 2, // right
    3, 2, 5, 3, 5, 4, // top
    7, 6, 1, 7, 1, 0  // bottom 
  };
  const unsigned nIndices = indices.size();
  buffer.Create(
    vertices.data(),
    nFloats,
    indices.data(),
    nIndices,
    { {3, 0}/*position*/, {3, 1}/*normal*/ }
  );

  const glm::vec3 origin{ 0.0, 0.0, 0.0 };
  cameraPos = { 0.0, 0.0, 5.0 };
  cameraDir = glm::normalize(origin - cameraPos);

  materialAmbient = { 1.0f, 0.0f, 1.0f };
  materialDiffuse = { 1.0f, 0.0f, 1.0f };
  materialSpecular = { 1.0f, 0.0f, 1.0f };
  materialShininess = 32.0f;

  for (size_t i = 0; i < 4; ++i) {
    Light& light = lights[i];

    light.ambient = { 0.2f, 0.2f, 0.2f };
    light.diffuse = { 0.5f, 0.5f, 0.5f };
    light.specular = { 1.0f, 1.0f, 1.0f };
    light.direction = { 0.0f, 0.0f, 1.0f };

    light.position = { 0.0f, 0.0f, -5.0f };
    light.quadratic = 0.2f;
    light.linear = 0.22f;
    light.constant = 1.0f;

    light.enabled = false;
    light.isDirectional = true;
  }

  lights[0].enabled = true;

  glEnable(GL_DEPTH_TEST);
}

//-----------------------------------------------------------------------------

void LightStudioApp::Render()
{
  shader.SetUniform("material.ambient", materialAmbient);
  shader.SetUniform("material.diffuse", materialDiffuse);
  shader.SetUniform("material.specular", materialSpecular);
  shader.SetUniform("material.shininess", materialShininess);

  int nDirectionalLights = 0;
  int nPointLights = 0;

  for (size_t i = 0; i < 4; ++i) {
    Light& light = lights[i];
    if (!light.enabled) {
      continue;
    }

    if (light.isDirectional) {
      const std::string iLight = "directionalLights[" + std::to_string(nDirectionalLights) + "].";
      shader.SetUniform(std::string(iLight + "ambient").c_str(), light.ambient);
      shader.SetUniform(std::string(iLight + "diffuse").c_str(), light.diffuse);
      shader.SetUniform(std::string(iLight + "specular").c_str(), light.specular);
      shader.SetUniform(std::string(iLight + "direction").c_str(), light.direction);

      ++nDirectionalLights;
    }
    else {
      const std::string iLight = "pointLights[" + std::to_string(nPointLights) + "].";
      shader.SetUniform(std::string(iLight + "ambient").c_str(), light.ambient);
      shader.SetUniform(std::string(iLight + "diffuse").c_str(), light.diffuse);
      shader.SetUniform(std::string(iLight + "specular").c_str(), light.specular);
      shader.SetUniform(std::string(iLight + "position").c_str(), light.position);
      shader.SetUniform(std::string(iLight + "quadratic").c_str(), light.quadratic);
      shader.SetUniform(std::string(iLight + "linear").c_str(), light.linear);
      shader.SetUniform(std::string(iLight + "constant").c_str(), light.constant);

      ++nPointLights;
    }
  }

  shader.SetUniform("nDirectionalLights", nDirectionalLights);
  shader.SetUniform("nPointLights", nPointLights);

  shader.SetUniform("viewPosition", cameraPos);

  const glm::mat4x4 view = glm::lookAt(cameraPos, cameraDir * 10.0f, { 0.0, 1.0, 0.0 });
  const glm::mat4x4 proj = glm::perspective(glm::radians(60.0f), (float)1280 / 960, 0.1f, 1000.0f);
  const glm::mat4x4 cameraTransform = proj * view;

  shader.SetUniform("cameraTransform", cameraTransform);

  shader.Use();
  buffer.Render();
}

//-----------------------------------------------------------------------------

void LightStudioApp::Update(
  AppToolkit::IAppUtils& appUtils
)
{
  constexpr glm::vec2 screenCenter(1280 / 2, 960 / 2);
  if (appUtils.IsPressed(AppToolkit::MouseButton::RIGHT)) {
    appUtils.SetMousePos(screenCenter);
  }
  if (appUtils.IsHold(AppToolkit::MouseButton::RIGHT)) {
    const glm::vec3 zAxis = glm::normalize(cameraDir);
    glm::vec3 up{ 0.0f, 1.0f, 0.0f };
    if (1.0f - glm::abs(glm::dot(zAxis, up)) < 0.01f) {
      up = { 1.0f, 0.0f, 0.0f };
    }

    const glm::vec3 xAxis = glm::normalize(glm::cross(zAxis, up));
    const glm::vec3 yAxis = glm::normalize(glm::cross(zAxis, xAxis));

    const glm::vec2 pos = appUtils.GetMousePos();
    const glm::vec2 offset = pos - screenCenter;

    appUtils.SetMousePos(screenCenter);

    if (glm::abs(offset.x) > 0) {
      glm::mat4x4 rot = glm::rotate(glm::identity<glm::mat4x4>(), glm::radians(offset.x), yAxis);
      glm::vec4 p(cameraPos, 1.0f);
      cameraPos = rot * p;
    }
    if (glm::abs(offset.y) > 0) {
      glm::mat4x4 rot = glm::rotate(glm::identity<glm::mat4x4>(), glm::radians(offset.y), -xAxis);
      glm::vec4 p(cameraPos, 1.0f);
      cameraPos = rot * p;
    }

    constexpr glm::vec3 originPoint(0.0f, 0.0f, 0.0f);
    cameraDir = glm::normalize(originPoint - cameraPos);
  }
}

//-----------------------------------------------------------------------------

void LightStudioApp::Present()
{
  ImGui::Begin("Material");
    ImGui::ColorEdit3("ambient", glm::value_ptr(materialAmbient));
    ImGui::ColorEdit3("diffuse", glm::value_ptr(materialDiffuse));
    ImGui::ColorEdit3("specular", glm::value_ptr(materialSpecular));
    ImGui::InputFloat("shininess", &materialShininess);
  ImGui::End();

  for (size_t i = 0; i < 4; ++i) {
    const std::string name = "Light " + std::to_string(i + 1);
    Light& light = lights[i];

    ImGui::Begin(name.c_str());
      ImGui::Checkbox("enabled", &light.enabled);
      ImGui::Checkbox("isDirectional", &light.isDirectional);
    
      ImGui::ColorEdit3("ambient", glm::value_ptr(light.ambient));
      ImGui::ColorEdit3("diffuse", glm::value_ptr(light.diffuse));
      ImGui::ColorEdit3("specular", glm::value_ptr(light.specular));

      if (light.isDirectional) {
        ImGui::InputFloat3("direction", glm::value_ptr(light.direction));
      }
      else {
        ImGui::InputFloat3("position", glm::value_ptr(light.position));
        ImGui::InputFloat("quadratic", &light.quadratic);
        ImGui::InputFloat("linear", &light.linear);
        ImGui::InputFloat("constant", &light.constant);
      }
    ImGui::End();
  }
}

//-----------------------------------------------------------------------------

void LightStudioApp::Cleanup()
{
  buffer.Delete();
  shader.Delete();
}

//-----------------------------------------------------------------------------
