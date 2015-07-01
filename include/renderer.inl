// Include this so my editor doesn't give me lots of compiler errors
#include "renderer.h"
#include "vertex.h"

#define VERTEX_BUFFER 0
#define INDEX_BUFFER 1
#define NUM_BUFFERS 2
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace surface {

template <typename T>
Renderer<T>::Renderer(const T& object) :
  instanced(false),
  object(object),
  buffers(2, 0) {
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  buffers.resize(2);
  glGenBuffers(NUM_BUFFERS, buffers.data());

  numIndices = object.getIndices().size();
  bufferIndices(object.getIndices());
  bufferVertices(object.getVertices());
  for (unsigned int i = 0; i < Vertex::NUM_ATTRIBUTES; i++) {
    addAttribute(i);
  }
  // Now construct the VAO...
  glBindVertexArray(0);
}

template <typename T>
void Renderer<T>::updateBuffers() {
  bufferVertices(object.getVertices());
}

template <typename T>
void Renderer<T>::addAttribute(unsigned int attrib) {
  glEnableVertexAttribArray(attrib);
  glVertexAttribPointer(
    attrib,
    Vertex::getSize(attrib),
    GL_FLOAT, GL_FALSE,
    sizeof(typename T::vertexType),
    BUFFER_OFFSET(Vertex::getOffset(attrib)));
}

template <typename T>
void Renderer<T>::addMatrixAttribute(unsigned int buffer,
                                     unsigned int location) {
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  for (unsigned int i = 0; i < 4 ; i++) {
    glEnableVertexAttribArray(location + i);
    glVertexAttribPointer(location + i, 4, GL_FLOAT, GL_FALSE,
                          sizeof(glm::mat4),
                          (const GLvoid*)(sizeof(GLfloat) * i * 4));
    glVertexAttribDivisor(location + i, 1);
  }
}

template <typename T>
template <typename V>
void Renderer<T>::bufferVertices(const std::vector<V>& vec) {
  glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTEX_BUFFER]);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(V) * vec.size(),
               vec.data(),
               GL_STREAM_DRAW);
}

template <typename T>
void Renderer<T>::bufferIndices(
    const std::vector<unsigned int>& indices) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEX_BUFFER]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(unsigned int) * indices.size(),
               indices.data(),
               GL_STATIC_DRAW);
}

template <typename T>
void Renderer<T>::draw(DrawType type) const {
  glBindVertexArray(vao);
  glDrawElements(static_cast<GLenum>(type),
                 numIndices,
                 GL_UNSIGNED_INT,
                 NULL);
  glBindVertexArray(0);
}

template <typename T>
void Renderer<T>::drawInstanced(
    const std::vector<glm::mat4>& wvs,
    const std::vector<glm::mat4>& wvps,
    DrawType drawType) {

  glBindVertexArray(vao);
  // First time instanced is called, set up buffers
  if (!instanced) {
    buffers.resize(4);
    glGenBuffers(2, &buffers[2]);
    addMatrixAttribute(buffers[2], 4);
    addMatrixAttribute(buffers[3], 8);
    instanced = true;
  }

  // TOOD Tidy this up
  glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(glm::mat4) * wvs.size(),
               wvs.data(),
               GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, buffers[3]);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(glm::mat4) * wvps.size(),
               wvps.data(),
               GL_DYNAMIC_DRAW);

  glDrawElementsInstanced(static_cast<GLenum>(drawType),
                          numIndices,
                          GL_UNSIGNED_INT,
                          nullptr,
                          wvps.size());
  glBindVertexArray(0);


}

}
