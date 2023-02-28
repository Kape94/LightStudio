#ifndef _FLAT_SHADING_EFFECT_H
#define _FLAT_SHADING_EFFECT_H

#include "IEffect.h"
#include "Shader.h"

class FlatShadingEffect : public IEffect {
  public:

    void Create() override;

    void Cleanup() override;

    void Use(
      const LightModel& model,
      const Camera& camera
    ) override;

    void PostProcess(
      const LightModel& model,
      const Camera& camera
    ) override {}

    std::string Name() const override;

  private:

    Shader shader;
};

#endif
