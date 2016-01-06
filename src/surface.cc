#include "surface.h"
#include <iostream>
#include <algorithm>
#include <numeric>

namespace surface {
  Surface::Surface(unsigned int rows, unsigned int cols) :
      rows(rows), cols(cols) {
    // Set up vertices
    for (unsigned int row = 0; row <= rows; row++) {
      for (unsigned int col = 0; col <= cols; col++) {
        float s = static_cast<float>(row) / rows;
        float t = static_cast<float>(col) / cols;
        float x = static_cast<float>(row * 2) / rows - 1;
        float z = static_cast<float>(col * 2) / cols - 1;
        vertices.push_back(Vertex{ glm::vec3(x, 0.0f, z),
                                   glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                                   glm::vec2(s, t),
                                   glm::vec3(0.0f, 1.0f, 0.0f) });
      }
    }
    // And indices
    for (unsigned int row = 0; row < rows; row++) {
      for (unsigned int col = 0; col < cols; col++) {
        indices.push_back(getVertex(row,     col));
        indices.push_back(getVertex(row,     col + 1));
        indices.push_back(getVertex(row + 1, col));
        indices.push_back(getVertex(row + 1, col));
        indices.push_back(getVertex(row,     col + 1));
        indices.push_back(getVertex(row + 1, col + 1));
      }
    }
  }

  unsigned int Surface::getVertex(unsigned int x, unsigned int y) {
    return x * (cols + 1) + y;
  };


  glm::vec3 calcNormal(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
    return glm::normalize(glm::cross(glm::normalize(b - a), glm::normalize(c - a)));
  }

  void Surface::updateNormals() {
    // Update normals
    for (unsigned int row = 0; row <= rows; row++) {
      for (unsigned int col = 0; col <= cols; col++) {
        std::vector<glm::vec3> normals;
        glm::vec3& v = vertices[getVertex(row, col)].pos;
        if (row > 0) {
          if (col > 0) {
            glm::vec3& a = vertices[getVertex(row - 1, col)].pos;
            glm::vec3& b = vertices[getVertex(row - 1, col - 1)].pos;
            normals.push_back(calcNormal(v, a, b));
            glm::vec3& c = vertices[getVertex(row, col - 1)].pos;
            normals.push_back(calcNormal(v, b, c));
          }
          if (col < cols) {
            glm::vec3& a = vertices[getVertex(row, col + 1)].pos;
            glm::vec3& b = vertices[getVertex(row - 1, col)].pos;
            normals.push_back(calcNormal(v, a, b));
          }
        }
        if (row < rows) {
          if (col > 0) {
            glm::vec3& a = vertices[getVertex(row, col - 1)].pos;
            glm::vec3& b = vertices[getVertex(row + 1, col)].pos;
            normals.push_back(calcNormal(v, a, b));
          }
          if (col < cols) {
            glm::vec3& a = vertices[getVertex(row + 1, col)].pos;
            glm::vec3& b = vertices[getVertex(row + 1, col + 1)].pos;
            normals.push_back(calcNormal(v, a, b));
            glm::vec3& c = vertices[getVertex(row, col + 1)].pos;
            normals.push_back(calcNormal(v, b, c));
          }
        }
        glm::vec3 tot = std::accumulate(normals.begin(), normals.end(), glm::vec3(0.0f));
        vertices[getVertex(row, col)].normal = - tot / static_cast<float>(normals.size());
      }
    }
  }

  Surface::Surface(const glm::vec2& size) :
    Surface(size.x, size.y) {
  }

  unsigned int Surface::getRows() {
    return rows;
  }

  unsigned int Surface::getCols() {
    return cols;
  }
}
