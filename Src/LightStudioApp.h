#ifndef _LIGHT_STUDIO_APP_H
#define _LIGHT_STUDIO_APP_H

#include "AppToolkit/IAppCleanup.h"
#include "AppToolkit/IAppInitializer.h"
#include "AppToolkit/IAppRenderer.h"
#include "AppToolkit/IAppUpdater.h"
#include "AppToolkit/IuiPresenter.h"

#include <glm/vec3.hpp>

#include "Buffer.h"
#include "Camera.h"
#include "LightModel.h"
#include "PhongPipeline.h"

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

    LightModel model;
    PhongPipeline pipeline;
    Camera camera;
    Buffer buffer;
};

#endif
