#ifndef _APP_UI_H
#define _APP_UI_H

#include "EffectControlUI.h"
#include "LightControlsUI.h"
#include "ShapeControlUI.h"

class AppUI {
  public:

    void Present(
      LightModel& model,
      EffectManager& pipelineManager,
      ShapesManager& shapesManager
    );

  private:

    LightControlsUI lightsAndMaterialUI;
    EffectControlUI effectControlUI;
    ShapeControlUI shapeControlUI;
};

#endif
