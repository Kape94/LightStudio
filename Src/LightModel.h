#ifndef _LIGHT_MODEL_H
#define _LIGHT_MODEL_H

#include "Light.h"
#include "Material.h"

class LightModel {
  public:

    void InitializeDefaultParameters();

    Material& GetMaterial();

    const Material& GetMaterial() const;

    Light& GetLight(
      const unsigned index
    );

    const Light& GetLight(
      const unsigned index
    ) const;

  private:

    Material material;

    static constexpr unsigned MAX_LIGHTS = 4;
    Light lights[MAX_LIGHTS];
};

#endif
