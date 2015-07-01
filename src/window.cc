#include "window.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

namespace surface {

Window::Window(const Options& options) :
  mouseClicked(false) {
  if (!glfwInit()) {
    throw std::runtime_error("Could not initialise GLFW\n");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  auto windowSize = options.getWindowSize();
  window = glfwCreateWindow(windowSize.x,
                            windowSize.y,
                            "OpenGL Window", NULL, NULL);
  width = windowSize.x, height = windowSize.y;
  if (!window) {
    throw std::runtime_error("Could not initialise GLFW window\n");
  }
  glfwMakeContextCurrent(window);
  // Set up callbacks
  glfwSetWindowUserPointer(window, this);
  auto mouseMove = [](GLFWwindow* w, double x, double y) {
      static_cast<Window*>(glfwGetWindowUserPointer(w))->mouseMove(x, y);
  };
  glfwSetCursorPosCallback(window, mouseMove);
  auto mouseClick = [](GLFWwindow* w, int button, int action, int) {
    if (action == GLFW_PRESS) {
      static_cast<Window*>(glfwGetWindowUserPointer(w))->mouseClick(button);
    }
  };
  glfwSetMouseButtonCallback(window, mouseClick);
  auto keyboard = [](GLFWwindow* w, int key, int, int action, int) {
      static_cast<Window*>(glfwGetWindowUserPointer(w))->keyboard(key, action);
  };
  glfwSetKeyCallback(window, keyboard);

  auto resize = [&](GLFWwindow* w, int width, int height) {
    static_cast<Window*>(glfwGetWindowUserPointer(w))->resize(width, height);
  };
  glfwSetWindowSizeCallback(window, resize);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    throw std::runtime_error("Could not initialise GLEW.\n");
  }
  glGetError();
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glEnable( GL_BLEND );
}

Window::~Window() {
  glfwDestroyWindow(window);
}

bool Window::isOpen() const {
  glfwPollEvents();
  return !glfwWindowShouldClose(window);
}

void Window::swapBuffers() const {
  glfwSwapBuffers(window);
}

void Window::clear() const {
  glViewport(0, 0, width, height);
  //glClearColor(0, 0, 0, 0);
  glClearColor(0.529, 0.808, 0.980, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::keyboard(int key, int action) {
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

void Window::resize(int w, int h) {
  width = w, height = h;
  glViewport(0, 0, width, height);
}

unsigned int Window::getHeight() const {
  return height;
}
unsigned int Window::getWidth() const {
  return width;
}

void Window::mouseClick(int button) {
  mouseClicked = true;
}

void Window::mouseMove(double x, double y) {
}

bool Window::getMouseClicked() {
  if (mouseClicked) {
    mouseClicked = false;
    return true;
  }
  return false;
}

}
