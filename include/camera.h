#pragma once

#include <glm/glm.hpp>

namespace surface {

class Camera {
  public:
    Camera(const glm::vec3& eye,
           const glm::vec3& up);

    Camera(const glm::vec3& eye,
           const glm::vec3& target,
           const glm::vec3& up);

    const glm::mat4& getViewMatrix() const;

    void moveRound(const glm::vec3& point, double angle);
    void moveForward(double step);
    void moveBackwards(double step);
    void moveLeft(double step);
    void moveRight(double step);
    void rotateUp(double d);
    void rotateDown(double d);
    void rotateLeft(double d);
    void rotateRight(double d);
  private:
    glm::vec3 eye, up;
    double rLat, rLon;

    void updateView();
    glm::mat4 view;
};

}
