#ifndef _CAMERA_H
#define _CAMERA_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Camera {
  public:

    void SetPosition(
      const glm::vec3& pos
    );
    void LookAtPoint(
      const glm::vec3& pos
    );

    glm::vec3 GetPosition() const;
    glm::vec3 GetDirection() const;
    glm::mat4x4 CreateTransformationMatrix() const;

  private:

    glm::vec3 position;
    glm::vec3 direction;
};

#endif
