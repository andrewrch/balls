#include "mesh.h"

namespace surface {

template <typename T>
Mesh<T>::Mesh() : vertices(0), indices(0) {
}

template <typename T>
Mesh<T>::Mesh(const std::vector<T>& v,
              const std::vector<unsigned int>& i) :
  vertices(v), indices(i) {
}

template <typename T>
void Mesh<T>::setVertices(const std::vector<T>& v) {
  vertices = v;
}

template <typename T>
const std::vector<T>& Mesh<T>::getVertices() const {
  return vertices;
}

template <typename T>
const std::vector<unsigned int>& Mesh<T>::getIndices() const {
  return indices;
}

}
