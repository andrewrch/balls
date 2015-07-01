#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace surface {

class Ball {
  public:
    explicit Ball();
    explicit Ball(const glm::vec3& pos, double radius, const glm::quat&);

    void update(const glm::vec3& acc, double timeStep);

    const glm::vec3& getPosition() const;
    double getRadius() const;
    const glm::quat& getRotation() const;

    void setPosition(const glm::vec3&);

    void bounce(const glm::vec3&);
    bool stopped() const;
    bool intersects(const Ball&) const;
  private:
    glm::vec3 pos, vel;;
    double radius;
    glm::quat rot, angVel;
};
}
