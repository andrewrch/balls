#include <assert.h>
#include <cmath>
#include <iostream>
#include <stdlib.h>

#include "sphere.h"

namespace surface {

Sphere::Sphere(double radius, unsigned int rings, unsigned int slices) {
    init(radius, rings, slices);
}

void Sphere::init(double radius, unsigned int rings, unsigned int slices) {
  float const R = 1.0f/(float)(rings-1);
  float const S = 1.0f/(float)(slices-1);
  unsigned int r, s;

  for(r = 0; r < rings; r++) {
    for(s = 0; s < slices; s++) {
      float const y = sin( -M_PI_2 + M_PI * r * R );
      float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
      float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );
      vertices.push_back(Vertex{ .pos = glm::vec3(x * radius, y * radius, z * radius),
                                 .colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
                                 .tex_coord = glm::vec2(s * S, r * R),
                                 .normal = glm::vec3(x, y, z) });
    }
  }

  for(r = 0; r < rings-1; r++) {
    for(s = 0; s < slices; s++) {
      indices.push_back(r * slices + s);
      indices.push_back((r+1) * slices + s);
      indices.push_back(r * slices + (s+1));
      indices.push_back(r * slices + (s+1));
      indices.push_back((r+1) * slices + s);
      indices.push_back((r+1) * slices + (s+1));
    }
  }
}

}
