#ifndef _LIGHT_STUDIO_APP_H
#define _LIGHT_STUDIO_APP_H

#include "AppToolkit/IAppCleanup.h"
#include "AppToolkit/IAppInitializer.h"
#include "AppToolkit/IAppRenderer.h"
#include "AppToolkit/IAppUpdater.h"
#include "AppToolkit/IuiPresenter.h"

#include <glm/vec3.hpp>

#include "AppUI.h"
#include "Buffer.h"
#include "Camera.h"
#include "CameraController.h"
#include "EffectManager.h"
#include "LightModel.h"
#include "ShapesManager.h"

#define IMPLEMENTS public

class LightStudioApp : 
  IMPLEMENTS AppToolkit::IAppInitializer,
  IMPLEMENTS AppToolkit::IAppRenderer,
  IMPLEMENTS AppToolkit::IAppUpdater,
  IMPLEMENTS AppToolkit::IuiPresenter,
  IMPLEMENTS AppToolkit::IAppCleanup
{
  public:

    void Initialize() override;
    void Render() override;
    void Update(AppToolkit::IAppUtils& appUtils) override;
    void Present() override;
    void Cleanup() override;

  private:

    EffectManager effectManager;
    ShapesManager shapesManager;

    AppUI ui;

    LightModel model;
    Camera camera;
    CameraController cameraController;
    Buffer buffer;


};

#endif
