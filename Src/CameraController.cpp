#include "CameraController.h"

#include <glm/gtc/matrix_transform.hpp>

//---------------------------------------------------------------------------------------

void CameraController::Update(
  Camera& camera, 
  AppToolkit::IAppUtils& appUtils
)
{
  constexpr glm::vec2 screenCenter(1280 / 2, 960 / 2);
  if (appUtils.IsPressed(AppToolkit::MouseButton::RIGHT)) {
    appUtils.SetMousePos(screenCenter);
  }
  if (appUtils.IsHold(AppToolkit::MouseButton::RIGHT)) {
    HandleMouseMove(camera, appUtils, screenCenter);
  }
}

//---------------------------------------------------------------------------------------

void CameraController::HandleMouseMove(
  Camera& camera,
  AppToolkit::IAppUtils& appUtils,
  const glm::vec2& screenCenter
) const
{
  glm::vec3 xAxis, yAxis, zAxis;
  FillCameraAxis(camera, xAxis, yAxis, zAxis);

  const glm::vec2 pos = appUtils.GetMousePos();
  const glm::vec2 offset = pos - screenCenter;
  appUtils.SetMousePos(screenCenter);

  MoveCameraWithCursorOffset(camera, offset, xAxis, yAxis);

  camera.LookAtPoint({ 0.0f, 0.0f, 0.0f });
}

//---------------------------------------------------------------------------------------

void CameraController::FillCameraAxis(
  const Camera& camera,
  glm::vec3& xAxis,
  glm::vec3& yAxis,
  glm::vec3& zAxis
) const
{
  zAxis = glm::normalize(camera.GetDirection());
  
  glm::vec3 up{ 0.0f, 1.0f, 0.0f };
  const bool upIsParallelToZAxis = 1.0f - glm::abs(glm::dot(zAxis, up)) < 0.01f;
  if (upIsParallelToZAxis) {
    up = { 1.0f, 0.0f, 0.0f };
  }

  xAxis = glm::normalize(glm::cross(zAxis, up));
  yAxis = glm::normalize(glm::cross(zAxis, xAxis));
}

//---------------------------------------------------------------------------------------

void CameraController::MoveCameraWithCursorOffset(
  Camera& camera,
  const glm::vec2& offset,
  const glm::vec3& xAxis,
  const glm::vec3& yAxis
) const
{
  if (glm::abs(offset.x) > 0) {
    glm::mat4x4 rot = glm::rotate(glm::identity<glm::mat4x4>(), glm::radians(offset.x), yAxis);
    glm::vec4 p(camera.GetPosition(), 1.0f);
    camera.SetPosition(rot * p);
  }
  if (glm::abs(offset.y) > 0) {
    glm::mat4x4 rot = glm::rotate(glm::identity<glm::mat4x4>(), glm::radians(offset.y), -xAxis);
    glm::vec4 p(camera.GetPosition(), 1.0f);
    camera.SetPosition(rot * p);
  }
}

//---------------------------------------------------------------------------------------
