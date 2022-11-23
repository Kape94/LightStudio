#ifndef _PHONG_PIPELINE_H
#define _PHONG_PIPELINE_H

#include "Camera.h"
#include "LightModel.h"
#include "Shader.h"

class PhongPipeline {
  public:

    void Create();
    void Cleanup();

    void Use(
      const LightModel& model,
      const Camera& camera
    );

  private:

    Shader shader;
};

#endif
