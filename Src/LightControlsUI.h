#ifndef _LIGHT_CONTROLS_UI_H
#define _LIGHT_CONTROLS_UI_H

#include "Light.h"
#include "LightModel.h"

#include <string>

class LightControlsUI {
  public:

    void PresentUI(
      LightModel& model
    ) const;

  private:

    void PresentMaterialUI(
      LightModel& model
    ) const;

    void PresentLightsUI(
      LightModel& model
    ) const;

    void PresentLightUI(
      Light& light, 
      const std::string& title
    ) const;

    void PresentBasicLightInfo(
      Light& light
    ) const;

    void PresentDirectionalLightInfo(
      Light& light
    ) const;

    void PresentPointLightInfo(
      Light& light
    ) const;
};

#endif
