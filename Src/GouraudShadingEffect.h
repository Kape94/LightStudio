#ifndef _GOURAUD_SHADING_EFFECT_H
#define _GOURAUD_SHADING_EFFECT_H

#include "IEffect.h"
#include "Shader.h"

class GouraudShadingEffect : public IEffect {
  public:

    void Create() override;

    void Cleanup() override;

    void Use(
      const LightModel& model,
      const Camera& camera
    ) override;

    std::string Name() const override;

  private:

    Shader shader;
};

#endif
