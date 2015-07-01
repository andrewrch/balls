#pragma once

#include <vector>
#include "mesh.h"
#include "vertex.h"
#include <glm/glm.hpp>

namespace surface {

class Sphere : public Mesh<Vertex> {
  public:
    /**
     * GLSphere constructor.
     *
     * @param radius The radius of the sphere.
     * @param rings The number of rings in the sphere.
     * @param sectors The number of sectors in each ring.
     */
    Sphere(double radius, unsigned int rings, unsigned int sectors);

  private:
    void init(double radius, unsigned int rings, unsigned int sectors);
    static unsigned int calcNumIndices(unsigned int rings,
                                       unsigned int sectors) {
      return (rings - 1) * sectors * 6;
    }
    static unsigned int calcNumVertices(unsigned int rings,
                                        unsigned int sectors) {
      return rings * sectors;
    }
};

}
