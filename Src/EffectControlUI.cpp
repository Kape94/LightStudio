#include "EffectControlUI.h"

#include "imgui.h"

//---------------------------------------------------------------------------------------

void EffectControlUI::PresentUI(
  EffectManager& manager
)
{
  ImGui::Begin("Lightning model");
  PresentEffectsCombo(manager);
  ImGui::End();
}

//---------------------------------------------------------------------------------------

void EffectControlUI::PresentEffectsCombo(
  EffectManager& manager
)
{
  const std::string currentEffectName = manager.GetCurrentEffect().Name();

  if (ImGui::BeginCombo("Model", currentEffectName.c_str())) {
    for (const std::unique_ptr<IEffect>& effect : manager.Effects()) {
      HandleItem(*effect, currentEffectName, manager);
    }
    ImGui::EndCombo();
  }
}

//---------------------------------------------------------------------------------------

void EffectControlUI::HandleItem(
  const IEffect& effect,
  const std::string& currentEffectName,
  EffectManager& manager
)
{
  const std::string effectName = effect.Name();
  const bool isSelected = effectName == currentEffectName;

  if (ImGui::Selectable(effectName.c_str(), isSelected)) {
    manager.SelectEffectByName(effectName);
  }
  if (isSelected) {
    ImGui::SetItemDefaultFocus();
  }
}

//---------------------------------------------------------------------------------------
