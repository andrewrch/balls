#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "vertex.h"

namespace surface {

template <typename VertexType>
class Mesh {
  public:
    explicit Mesh();
    explicit Mesh(const std::vector<VertexType>& v,
                  const std::vector<unsigned int>& i);

    const std::vector<VertexType>& getVertices() const;
    const std::vector<unsigned int>& getIndices() const;

    void setVertices(const std::vector<VertexType>& vertices);

    typedef VertexType vertexType;
  protected:
    std::vector<VertexType> vertices;
    std::vector<unsigned int> indices;
};

}

#include "mesh.inl"
