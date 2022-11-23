#include "LightStudioApp.h"

#include "Buffer.h"
#include "Shader.h"

#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

//-----------------------------------------------------------------------------

void LightStudioApp::Initialize()
{
  pipeline.Create();

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

  camera.SetPosition({ 0.0, 0.0, 5.0 });
  camera.LookAtPoint({ 0.0, 0.0, 0.0 });

  model.InitializeDefaultParameters();

  glEnable(GL_DEPTH_TEST);
}

//-----------------------------------------------------------------------------

void LightStudioApp::Render()
{
  pipeline.Use(model, camera);
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
    const glm::vec3 zAxis = glm::normalize(camera.GetDirection());
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
      glm::vec4 p(camera.GetPosition(), 1.0f);
      camera.SetPosition(rot * p);
    }
    if (glm::abs(offset.y) > 0) {
      glm::mat4x4 rot = glm::rotate(glm::identity<glm::mat4x4>(), glm::radians(offset.y), -xAxis);
      glm::vec4 p(camera.GetPosition(), 1.0f);
      camera.SetPosition(rot * p);
    }

    constexpr glm::vec3 originPoint(0.0f, 0.0f, 0.0f);
    camera.LookAtPoint(originPoint);
  }
}

//-----------------------------------------------------------------------------

void LightStudioApp::Present()
{
  ImGui::Begin("Material");
    Material& material = model.GetMaterial();
    ImGui::ColorEdit3("ambient", glm::value_ptr(material.ambient));
    ImGui::ColorEdit3("diffuse", glm::value_ptr(material.diffuse));
    ImGui::ColorEdit3("specular", glm::value_ptr(material.specular));
    ImGui::InputFloat("shininess", &material.shininess);
  ImGui::End();

  for (size_t i = 0; i < 4; ++i) {
    const std::string name = "Light " + std::to_string(i + 1);
    Light& light = model.GetLight(i);

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
  pipeline.Cleanup();
}

//-----------------------------------------------------------------------------
