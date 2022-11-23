#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

//---------------------------------------------------------------------------------------

void Camera::SetPosition(
  const glm::vec3& pos
)
{
  position = pos;
}

//---------------------------------------------------------------------------------------

void Camera::LookAtPoint(
  const glm::vec3& lookAtPos
)
{
  direction = glm::normalize(lookAtPos - position);
}

//---------------------------------------------------------------------------------------

glm::vec3 Camera::GetPosition() const
{
  return position;
}

//---------------------------------------------------------------------------------------

glm::vec3 Camera::GetDirection() const
{
  return direction;
}

//---------------------------------------------------------------------------------------

glm::mat4x4 Camera::CreateTransformationMatrix() const
{
  constexpr glm::vec3 up{ 0.0, 1.0, 0.0 };
  const glm::mat4x4 view = glm::lookAt(position, direction * 10.0f, up);

  const glm::mat4x4 projection = glm::perspective(glm::radians(60.0f), (float)1280 / 960, 0.1f, 1000.0f);
  return projection * view;
}

//---------------------------------------------------------------------------------------
