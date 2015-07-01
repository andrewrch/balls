#pragma once

#include "window.h"
#include "options.h"
#include "surface.h"
#include "renderer.h"
#include "camera.h"
#include "sphere.h"
#include "shader_program.h"
#include "ball_pool.h"
#include "shadow_map.h"
#include "texture.h"

namespace surface {
  /**
   * An instance of this class will do all of the work
   */
  class SurfaceApp {
    public:
      SurfaceApp(Window& window, const Options& options);

      /**
       * This is the program main loop.
       * Call this and the render loop begins.
       */
      void mainLoop();
    private:

      void drawLight();
      void shadowPass();
      void drawSurface();
      void drawBalls();

      void updateSurface(double);

      Window& window;
      Surface surface;
      Renderer<Surface> surfaceRenderer;
      ShaderProgram surfaceShader;
      Sphere sphere;
      Renderer<Sphere> sphereRenderer;
      ShaderProgram sphereShader;
      ShaderProgram lightShader;
      Camera camera;
      BallPool bp;
      glm::mat4 cameraProj;
      glm::vec3 lightPos;
      ShadowMap map;
      Texture ballTexture, surfaceTexture;
  };
}
