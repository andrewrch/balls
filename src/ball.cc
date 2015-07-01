#include "ball.h"
#include <iostream>
#include <glm/ext.hpp>

namespace surface {

  Ball::Ball(const glm::vec3& pos, double radius, const glm::quat& rot) :
    pos(pos), vel(0), radius(radius), rot(rot), angVel(1, 0, 0, 0)
  {}

  Ball::Ball() : Ball(glm::vec3(0), 1, glm::quat(1, 0, 0, 0))
  {}

  glm::quat deltaRotation(const glm::vec3& em, double deltaTime)
  {
    glm::quat q;
    glm::vec3 ha = em * static_cast<float>(deltaTime) * 0.5f; // vector of half angle
    double l = glm::length(ha); // magnitude
    if( l > 0 )
    {
       double ss = sin(l)/l;
       q = glm::quat(cos(l), ha.x*ss, ha.y*ss, ha.z*ss);
    }else{
       q = glm::quat(1.0, ha.x, ha.y, ha.z);
    }
    return q;
  }

  void Ball::bounce(const glm::vec3& dir) {
    // Decrease velocity (artbitrarily)
    vel *= 0.90;
    float len = glm::length(vel);
    vel = glm::normalize(dir) * len;
    // Make up a reasonable guess at a new spin direction
    angVel = glm::quat(-glm::normalize(dir) * 0.2f);

    // Update rotation and position
    rot = angVel * rot;
    pos += vel;
  }

  bool Ball::intersects(const Ball& ball) const {
    return glm::distance(ball.pos, pos) - (radius + ball.radius) <= 0;
  }

  void Ball::update(const glm::vec3& acc, double timeStep) {
    // Decrease angular velocity of 90% (So they don't spin forever)
    angVel = glm::normalize(glm::pow(angVel, 0.9f));
    // Update rotation and velocity
    rot = angVel * rot;
    vel += acc * static_cast<float>(timeStep);
    pos += vel;
  }

  bool Ball::stopped() const {
    return glm::length(vel) < 0.01;
  }

  void Ball::setPosition(const glm::vec3& p) {
    pos = p;
  }

  const glm::vec3& Ball::getPosition() const {
    return pos;
  }

  const glm::quat& Ball::getRotation() const {
    return rot;
  }

  double Ball::getRadius() const {
    return radius;
  }
}
