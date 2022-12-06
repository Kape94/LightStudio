#ifndef _EFFECT_MANAGER_H
#define _EFFECT_MANAGER_H

#include "IEffect.h"

#include <memory>
#include <string>
#include <vector>

class EffectManager {
  public:

    void Initialize();

    IEffect& GetCurrentEffect();

    void SelectEffectByName(
      const std::string& name
    );

    void Cleanup();

    const std::vector<std::unique_ptr<IEffect>>& Effects() const;

  private:

    IEffect* selected = nullptr;

    std::vector<std::unique_ptr<IEffect>> effects;
};

#endif
