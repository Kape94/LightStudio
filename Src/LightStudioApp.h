#ifndef _LIGHT_STUDIO_APP_H
#define _LIGHT_STUDIO_APP_H

#include "AppToolkit/IAppCleanup.h"
#include "AppToolkit/IAppInitializer.h"
#include "AppToolkit/IAppRenderer.h"
#include "AppToolkit/IAppUpdater.h"
#include "AppToolkit/IuiPresenter.h"

#include <glm/vec3.hpp>

#include "Light.h"

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

    glm::vec3 materialAmbient;
    glm::vec3 materialDiffuse;
    glm::vec3 materialSpecular;
    float materialShininess;

    Light lights[4];

    glm::vec3 lightAmbient;
    glm::vec3 lightDiffuse;
    glm::vec3 lightSpecular;
    glm::vec3 lightDirection;

    glm::vec3 cameraPos;
    glm::vec3 cameraDir;
};

#endif
