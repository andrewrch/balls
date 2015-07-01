#pragma once

#include <glm/glm.hpp>

namespace surface {

struct Vertex {

  // The data
  glm::vec3 pos;
  glm::vec4 colour;
  glm::vec2 tex_coord;
  glm::vec3 normal;

  // Static members
  const static int NUM_ATTRIBUTES = 4;
  static unsigned int getOffset(unsigned int attrib) {
    unsigned int o;
    switch (attrib) {
      case 0: o = 0;
              break;
      case 1: o = sizeof(glm::vec3);
              break;
      case 2: o = sizeof(glm::vec4) + getOffset(1);
              break;
      case 3: o = sizeof(glm::vec2) + getOffset(2);
              break;
      default: o = 0;
    }
    return o;
  }
  static unsigned int getSize(unsigned int attrib) {
    unsigned int s;
    switch (attrib) {
      case 0: s = 3;
              break;
      case 1: s = 4;
              break;
      case 2: s = 2;
              break;
      case 3: s = 3;
              break;
      default: s = 0;
    }
    return s;
  }
};

}
