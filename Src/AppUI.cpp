#include "AppUI.h"

//---------------------------------------------------------------------------------------

void AppUI::Present(
  LightModel& model, 
  EffectManager& pipelineManager,
  ShapesManager& shapesManager
)
{
  lightsAndMaterialUI.PresentUI(model);
  effectControlUI.PresentUI(pipelineManager);
  shapeControlUI.PresentUI(shapesManager);
}

//---------------------------------------------------------------------------------------
