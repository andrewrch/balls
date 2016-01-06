#include "camera.h"
#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#define M_PI 3.14

namespace surface {

Camera::Camera(const glm::vec3& eye, const glm::vec3& up) :
  eye(eye), up(up), rLat(0), rLon(0)
{
}

Camera::Camera(const glm::vec3& eye,
               const glm::vec3& target,
               const glm::vec3& up) :
  eye(eye), up(up) {
  auto dir = glm::normalize(target - eye);
  rLat = asin(dir.y);
  rLon = acos(dir.x / acos(dir.y));

  updateView();
}

void Camera::moveRound(const glm::vec3& point, double angle) {
  glm::mat4 mat(1.0f);
  mat = glm::translate(mat, point);
  mat = glm::rotate(mat, static_cast<float>(angle), glm::vec3(0, 1, 0));
  mat = glm::translate(mat, -point);
  eye = glm::vec3(mat * glm::vec4(eye, 1));
  view = glm::lookAt(eye, point, up);
}

void Camera::updateView() {
  // Calculate target
  glm::vec3 target;
  target.x = eye.x + cos(rLat) * cos(rLon);
  target.y = eye.y + sin(rLat);
  target.z = eye.z + sin(rLon) * cos(rLat);
  view = glm::lookAt(eye, target, up);
}

const glm::mat4& Camera::getViewMatrix() const {
  return view;
}

void Camera::moveForward(double step) {
  eye.x += step * cos(rLon);
  eye.y += 0;
  eye.z += step * sin(rLon);
  updateView();
}
void Camera::moveBackwards(double step) {
  eye.x -= step * cos(rLon);
  eye.y -= 0;
  eye.z -= step * sin(rLon);
  updateView();
}

void Camera::moveLeft(double step) {
  eye.x += step * sin(rLon);
  eye.y += 0;
  eye.z -= step * cos(rLon);
  updateView();
}

void Camera::moveRight(double step) {
  eye.x -= step * sin(rLon);
  eye.y += 0;
  eye.z += step * cos(rLon);
  updateView();
}

double degToRad(double d) {
  return (d * M_PI) / 180;
}

double checkBounds(double r) {
  if (r <= degToRad(-89)) {
    return degToRad(-89);
  } else if (r >= degToRad(89)) {
    return degToRad(89);
  } else {
    return r;
  }
}

void Camera::rotateUp(double d) {
  rLat += degToRad(d);
  rLat = checkBounds(rLat);
  updateView();
}
void Camera::rotateDown(double d) {
  rLat -= degToRad(d);
  rLat = checkBounds(rLat);
  updateView();
}
void Camera::rotateLeft(double d) {
  rLon -= degToRad(d);
  updateView();
}
void Camera::rotateRight(double d) {
  rLon += degToRad(d);
  updateView();
}

}
