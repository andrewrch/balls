#pragma once

#include "ball.h"
#include "surface.h"
#include <vector>
#include <random>

namespace surface {
  class BallPool {
    public:
      BallPool(unsigned int num);

      void addBall(const Ball& ball);
      void addBallRandom();
      void update (const Surface& surface, double timeStep);

      std::vector<glm::vec3> getBallPositions() const;
      const std::vector<Ball>& getBalls() const;
      const Ball& getClosest(const Ball&) const;

    private:

      glm::vec3 getRandomPos();
      glm::quat getRandomRot();
      double getRandomRadius();

      std::vector<Ball> balls;
      std::random_device gen;
      std::uniform_real_distribution<double> posDist;
      std::uniform_real_distribution<double> rotDist;
      std::uniform_real_distribution<double> radiusDist;
  };
}
