#include "surface_app.h"
#include "ball_pool.h"
#include "texture.h"
#include "shadow_map.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

namespace surface {
  SurfaceApp::SurfaceApp(Window& window, const Options& options) :
    window(window),
    surface(options.getSurfaceSize()),
    surfaceRenderer(surface),
    surfaceShader({ Shader(options.getSurfFragShader(), ShaderType::FRAG),
                    Shader(options.getSurfVertShader(), ShaderType::VERT) }),
    sphere(1.0, 20, 20),
    sphereRenderer(sphere),
    sphereShader({ Shader(options.getSurfFragShader(), ShaderType::FRAG),
                   Shader(options.getSurfVertShader(), ShaderType::VERT) }),
    lightShader({ Shader(options.getLightFragShader(), ShaderType::FRAG),
                  Shader(options.getLightVertShader(), ShaderType::VERT) }),
    camera(glm::vec3(2, 2, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)),
    bp(2),
    lightPos(1, 1, 0),
    map(512, 512, lightPos)
  {
    float ratio = static_cast<float>(window.getWidth()) / window.getHeight();
    cameraProj = glm::perspective(45.0f, ratio, 0.1f, 10000.0f);
    ballTexture.setImage("images/ball.jpg");
    surfaceTexture.setImage("images/grass.jpg");
  }

  void SurfaceApp::drawLight() {
    // Draw sphere for light
    lightShader.use();
    auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.05, 0.05, 0.05));
    auto trans = glm::translate(glm::mat4(1.0f), lightPos);
    auto w = trans * scale;
    auto wvp = cameraProj * camera.getViewMatrix() * w;
    lightShader.add_uniform("light_pos", lightPos);
    lightShader.add_uniform("WVP", wvp);
    lightShader.add_uniform("W", w);
    sphereRenderer.draw();
  }

  void SurfaceApp::shadowPass() {
    glClearColor(1e6, 1e6, 1e6, 1e6);
    for (int i = 0; i < 6; i++) {
      map.bind(i);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
      map.draw(i, surfaceRenderer, glm::mat4(1.0f));

      for (auto& b : bp.getBalls()) {
        double r = b.getRadius();
        auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(r, r, r));
        auto w = glm::translate(glm::mat4(1.0f), b.getPosition());
        map.draw(i, sphereRenderer, w * scale);
      }
    }
    map.unbind();
  }

  void SurfaceApp::drawSurface() {
    surfaceShader.use();
    auto w = glm::mat4(1.0f);
    auto wv = camera.getViewMatrix() * w;
    auto wvp = cameraProj * wv;
    surfaceShader.add_uniform("WVP", wvp);
    surfaceShader.add_uniform("WV", wv);
    surfaceShader.add_uniform("W", w);
    surfaceShader.add_uniform("N", glm::transpose(glm::inverse(wv)));
    surfaceTexture.bind();
    surfaceShader.add_uniform("surface", 0);
    surfaceShader.add_uniform("shadow", 1);
    surfaceShader.add_uniform("light_pos", lightPos);
    surfaceRenderer.draw();
    surfaceTexture.unbind();
  }

  void SurfaceApp::drawBalls() {
    ballTexture.bind();
    sphereShader.use();
    for (auto& b : bp.getBalls()) {
      double r = b.getRadius();
      auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(r, r, r));
      auto pos = glm::translate(glm::mat4(1.0f), b.getPosition());
      auto rot = b.getRotation();

      auto w = pos * glm::mat4_cast(rot) * scale;
      auto wv = camera.getViewMatrix() * w;
      auto wvp = cameraProj * wv;
      sphereShader.add_uniform("surface", 0);
      sphereShader.add_uniform("shadow", 1);
      sphereShader.add_uniform("light_pos", lightPos);
      sphereShader.add_uniform("WVP", wvp);
      sphereShader.add_uniform("WV", wv);
      sphereShader.add_uniform("W", w);
      sphereShader.add_uniform("N", glm::transpose(glm::inverse(wv)));
      sphereRenderer.draw();
    }
    ballTexture.unbind();
  }

  void SurfaceApp::updateSurface(double t) {
    // Update surface
    std::vector<Vertex> vertices;
    for (auto vertex : surface.getVertices()) {
      vertex.pos.y = cos(3 * (t + pow(vertex.pos.x, 2) +
                                  pow(vertex.pos.z, 2))) / 4;
      vertices.push_back(vertex);
    }
    surface.setVertices(vertices);
    surface.updateNormals();
    surfaceRenderer.updateBuffers();
  }

  void SurfaceApp::mainLoop() {
    double startTime = glfwGetTime(), prevTime = startTime;
    // Loop until window is closed or told to close
    while (window.isOpen()) {
      // Update projection in case window has been resized
      float ratio = static_cast<float>(window.getWidth()) / window.getHeight();
      cameraProj = glm::perspective(45.0f, ratio, 0.1f, 10000.0f);

      // Get new time for ball update
      double currentTime = glfwGetTime();
      if (window.getMouseClicked()) {
        bp.addBallRandom();
      }
      double timeStep = currentTime - prevTime;
      prevTime = currentTime;

      updateSurface(currentTime);

      lightPos.x = sin(currentTime);
      lightPos.z = cos(currentTime);
      map.setLightPos(lightPos);

      // Shadow pass
      shadowPass();

      // Render pass
      window.clear();

      drawLight();
      drawSurface();
      drawBalls();

      camera.moveRound(glm::vec3(0, 0, 0), 0.005);
      bp.update(surface, timeStep / 100);

      window.swapBuffers();
    }
  }
}
