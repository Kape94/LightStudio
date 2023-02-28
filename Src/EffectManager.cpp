#include "EffectManager.h"

#include "DeferredShadingEffect.h"
#include "FlatShadingEffect.h"
#include "GouraudShadingEffect.h"
#include "PhongShadingEffect.h"

#include <algorithm>

//---------------------------------------------------------------------------------------

void EffectManager::Initialize()
{
  effects.emplace_back(new PhongShadingEffect);
  effects.emplace_back(new GouraudShadingEffect);
  effects.emplace_back(new FlatShadingEffect);
  effects.emplace_back(new DeferredShadingEffect);

  for (auto& effect : effects) {
    effect->Create();
  }

  selected = effects.front().get();
}

//---------------------------------------------------------------------------------------

IEffect& EffectManager::GetCurrentEffect()
{
  return *selected;
}

//---------------------------------------------------------------------------------------

void EffectManager::SelectEffectByName(
  const std::string& name
)
{
  auto it = std::find_if(effects.begin(), effects.end(), [name](std::unique_ptr<IEffect>& effect) {
    return effect->Name() == name;
  });

  if (it != effects.end()) {
    selected = it->get();
  }
}

//---------------------------------------------------------------------------------------

void EffectManager::Cleanup()
{
  for (auto& effect : effects) {
    effect->Cleanup();
  }
}

//---------------------------------------------------------------------------------------

const std::vector<std::unique_ptr<IEffect>>& EffectManager::Effects() const
{
  return effects;
}

//---------------------------------------------------------------------------------------
