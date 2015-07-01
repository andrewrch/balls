#include "ball_pool.h"
#include <algorithm>
#include <iostream>
#include <glm/ext.hpp>

namespace surface {

BallPool::BallPool(unsigned int num) :
  posDist (-0.5, 0.5), rotDist(0.0, 1.0), radiusDist(0.03, 0.08) {

  // Insert first ball
  double radius = getRandomRadius();
  glm::vec3 pos = getRandomPos();
  glm::quat rot = getRandomRot();
  rot = glm::normalize(rot);
  Ball ball(pos, radius, rot);
  addBall(ball);

  for (unsigned int i = 1; i < num; i++) {
    addBallRandom();
  }
}

void BallPool::addBallRandom() {
  bool inserted = false;
  double radius = getRandomRadius();
  while (!inserted) {
    glm::vec3 pos = getRandomPos();
    glm::quat rot = getRandomRot();
    rot = glm::normalize(rot);
    Ball ball(pos, radius, rot);
    auto& closestBall = getClosest(ball);
    if (!closestBall.intersects(ball)) {
      balls.push_back(ball);
      inserted = true;
    }
  }
}

glm::vec3 BallPool::getRandomPos() {
  return glm::vec3(posDist(gen), 1, posDist(gen));
}

glm::quat BallPool::getRandomRot() {
  return glm::quat(rotDist(gen), rotDist(gen), rotDist(gen), rotDist(gen));
}

double BallPool::getRandomRadius() {
  return radiusDist(gen);
}

void BallPool::addBall(const Ball& ball) {
  balls.push_back(ball);
}

std::vector<glm::vec3> BallPool::getBallPositions() const {
  std::vector<glm::vec3> pos;
  for (auto& b : balls) {
    pos.push_back(b.getPosition());
  }
  return pos;
}

const Ball& BallPool::getClosest(const Ball& ball) const {
  // Find closest ball
  std::vector<Ball> otherBalls;
  for (auto& b : balls) {
    if (&b != &ball) {
      otherBalls.push_back(b);
    }
  }
  auto closestBall = std::min_element(otherBalls.begin(), otherBalls.end(),
    [&](const Ball& b1, const Ball& b2) {
      return glm::distance(ball.getPosition(), b1.getPosition()) <
             glm::distance(ball.getPosition(), b2.getPosition());
    });
  return *closestBall;
}

const std::vector<Ball>& BallPool::getBalls() const {
  return balls;
}

void BallPool::update(const Surface& surface, double timeStep) {
  // Make the balls bounce off the surface
  for (auto& ball : balls) {
    // Make some walls
    auto& pos = ball.getPosition();
    if (pos.x > 1) {
      ball.bounce(glm::vec3(-1, 0, 0));
    }
    if (pos.z > 1) {
      ball.bounce(glm::vec3(0, 0, -1));
    }
    if (pos.x < -1) {
      ball.bounce(glm::vec3(1, 0, 0));
    }
    if (pos.z < -1) {
      ball.bounce(glm::vec3(0, 0, 1));
    }

    // Else we are in the arena so update ball positions
    else {
      // Find closest point
      auto& vertices = surface.getVertices();
      auto closestPoint = std::min_element(vertices.begin(), vertices.end(),
        [&](const Vertex& v1, const Vertex& v2) {
          return glm::distance(v1.pos, ball.getPosition()) <
                 glm::distance(v2.pos, ball.getPosition());
        });

      // If we have hit the floor
      if (std::fabs(closestPoint->pos.y - ball.getPosition().y) - ball.getRadius() <= 0) {
        if (!ball.stopped()) {
          ball.bounce(closestPoint->normal);
          ball.update(glm::vec3(0, -4.8, 0), timeStep);
        } else {
          auto pos = ball.getPosition();
          pos.y = closestPoint->pos.y + ball.getRadius();
          ball.setPosition(pos);
          glm::vec3 norm = closestPoint->normal;
          //norm.y = -norm.y;
          ball.update(norm, timeStep);
        }
      } else {
        // If we've hit another ball
        auto& closestBall = getClosest(ball);
        if (closestBall.intersects(ball)) {
          ball.bounce(ball.getPosition() - closestBall.getPosition());
        }
        // Just update for G
        ball.update(glm::vec3(0, -4.8, 0), timeStep);
      }
    }
  }
}

}
