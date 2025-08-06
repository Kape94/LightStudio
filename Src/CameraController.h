#ifndef _CAMERA_CONTROLLER_H
#define _CAMERA_CONTROLLER_H

#include "Camera.h"

#include "AppToolkit/IAppUtils.h"

class CameraController {
  public:

    void Update(
      Camera& camera,
      AppToolkit::IAppUtils& appUtils
    );

  private:

    void EnterCameraControlMode(
      AppToolkit::IAppUtils& appUtils,
      const glm::vec2& screenCenter
    ) const;

    void HandleMouseMove(
      Camera& camera, 
      AppToolkit::IAppUtils& appUtils,
      const glm::vec2& screenCenter
    ) const;

    glm::vec2 GetCursorOffset(
      AppToolkit::IAppUtils& appUtils,
      const glm::vec2& screenCenter
    ) const;

    void FillCameraAxis(
      const Camera& camera,
      glm::vec3& xAxis,
      glm::vec3& yAxis,
      glm::vec3& zAxis
    ) const;

    void MoveCameraWithCursorOffset(
      Camera& camera, 
      const glm::vec2& offset,
      const glm::vec3& xAxis,
      const glm::vec3& yAxis
    ) const;
};

#endif
