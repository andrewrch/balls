#pragma once

#include "options.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace surface {
  class Window {
    public:
      Window(const Options& options);
      ~Window();

      Window& operator=(const Window&) = delete;
      Window(const Window& w) = delete;

      bool isOpen() const;
      void swapBuffers() const;
      void clear() const;

      // Callback handlers
      void render();
      void keyboard(int key, int action);
      void mouseMove(double x, double y);
      void mouseClick(int button);
      void resize(int, int);

      unsigned int getHeight() const;
      unsigned int getWidth() const;

      bool getMouseClicked();
    private:
      GLFWwindow* window;
      bool mouseClicked;
      unsigned int width, height;
  };
}
