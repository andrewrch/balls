#pragma once

#include "vertex.h"
#include "mesh.h"

namespace surface {
  class Surface : public Mesh<Vertex> {
    public:
      Surface(const glm::vec2& size);
      Surface(unsigned int rows, unsigned int cols);

      ~Surface() = default;
      Surface& operator=(const Surface&) = default;
      Surface(const Surface&) = default;

      void updateNormals();

      unsigned int getRows();
      unsigned int getCols();
    private:
      unsigned int getVertex(unsigned int row, unsigned int col);
      unsigned int rows, cols;
  };
}
