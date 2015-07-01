#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>

namespace surface {

enum class DrawType {
  TRIANGLES = GL_TRIANGLES,
  POINTS = GL_POINTS,
  LINES = GL_LINES
};

template <typename T>
class Renderer {
  public:
    Renderer(const T& object);
    void updateBuffers();

    void draw(DrawType = DrawType::TRIANGLES) const;

    void drawInstanced(
        const std::vector<glm::mat4>& wvs,
        const std::vector<glm::mat4>& wvps,
        DrawType = DrawType::TRIANGLES);

  private:
    GLuint vao;
    bool instanced; // Set to true if instance rendering called for
    const T& object;
    std::vector<GLuint> buffers;
    unsigned int numIndices;

    // Helper functions
    void bufferIndices(const std::vector<unsigned int>& indices);
    template <typename V>
    void bufferVertices(const std::vector<V>& vec);
    void addAttribute(unsigned int);
    void addMatrixAttribute(unsigned int buffer,
                            unsigned int attrib);
};
}

#include "renderer.inl"
