#include "LightControlsUI.h"

#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"

//---------------------------------------------------------------------------------------

void LightControlsUI::PresentUI(
  LightModel& model
) const
{
  PresentMaterialUI(model);
  PresentLightsUI(model);
}

//---------------------------------------------------------------------------------------

void LightControlsUI::PresentMaterialUI(
  LightModel& model
) const
{
  ImGui::Begin("Material");

  Material& material = model.GetMaterial();
  ImGui::ColorEdit3("ambient", glm::value_ptr(material.ambient));
  ImGui::ColorEdit3("diffuse", glm::value_ptr(material.diffuse));
  ImGui::ColorEdit3("specular", glm::value_ptr(material.specular));
  ImGui::InputFloat("shininess", &material.shininess);

  ImGui::End();
}

//---------------------------------------------------------------------------------------

void LightControlsUI::PresentLightsUI(
  LightModel& model
) const
{
  for (size_t i = 0; i < 4; ++i) {
    const std::string title = "Light " + std::to_string(i + 1);
    Light& light = model.GetLight(i);

    PresentLightUI(light, title);
  }
}

//---------------------------------------------------------------------------------------

void LightControlsUI::PresentLightUI(
  Light& light,
  const std::string& title
) const
{
  ImGui::Begin(title.c_str());

  PresentBasicLightInfo(light);
  if (light.isDirectional) {
    PresentDirectionalLightInfo(light);
  }
  else {
    PresentPointLightInfo(light);
  }

  ImGui::End();
}

//---------------------------------------------------------------------------------------

void LightControlsUI::PresentBasicLightInfo(
  Light& light
) const
{
  ImGui::Checkbox("enabled", &light.enabled);
  ImGui::Checkbox("isDirectional", &light.isDirectional);

  ImGui::ColorEdit3("ambient", glm::value_ptr(light.ambient));
  ImGui::ColorEdit3("diffuse", glm::value_ptr(light.diffuse));
  ImGui::ColorEdit3("specular", glm::value_ptr(light.specular));
}

//---------------------------------------------------------------------------------------

void LightControlsUI::PresentDirectionalLightInfo(
  Light& light
) const
{
  ImGui::InputFloat3("direction", glm::value_ptr(light.direction));
}

//---------------------------------------------------------------------------------------

void LightControlsUI::PresentPointLightInfo(
  Light& light
) const
{
  ImGui::InputFloat3("position", glm::value_ptr(light.position));
  ImGui::InputFloat("quadratic", &light.quadratic);
  ImGui::InputFloat("linear", &light.linear);
  ImGui::InputFloat("constant", &light.constant);
}

//---------------------------------------------------------------------------------------
