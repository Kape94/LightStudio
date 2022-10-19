#ifndef _LIGHT_STUDIO_APP_H
#define _LIGHT_STUDIO_APP_H

#include "AppToolkit/IAppInitializer.h"
#include "AppToolkit/IAppRenderer.h"

#define IMPLEMENTS public

class LightStudioApp : 
  IMPLEMENTS AppToolkit::IAppInitializer,
  IMPLEMENTS AppToolkit::IAppRenderer
{
  public:

    void Initialize() override;
    void Render() override;

  private:

};

#endif
