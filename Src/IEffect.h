#ifndef _I_EFFECT_H
#define _I_EFFECT_H

#include "Camera.h"
#include "LightModel.h"

#include <string>

class IEffect {
  public:

    virtual void Create() = 0;
    
    virtual void Cleanup() = 0;

    virtual void Use(
      const LightModel& model,
      const Camera& camera
    ) = 0;

    virtual void PostProcess(
      const LightModel& model,
      const Camera& camera
    ) = 0;

    virtual std::string Name() const = 0;
};

#endif
