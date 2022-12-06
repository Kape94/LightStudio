#ifndef _EFFECT_CONTROL_UI_H
#define _EFFECT_CONTROL_UI_H

#include "IEffect.h"
#include "EffectManager.h"

class EffectControlUI {
  public:

    void PresentUI(
      EffectManager& manager
    );

  private:

    void PresentEffectsCombo(
      EffectManager& manager
    );

    void HandleItem(
      const IEffect& effect, 
      const std::string& currentEffectName, 
      EffectManager& manager
    );
};

#endif
