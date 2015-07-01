#pragma once

#include <GL/glew.h>
#include <stdbool.h>
#include "texture.h"
#include "shader_program.h"
#include "mesh.h"

namespace surface {

class ShadowMap {

  public:
    ShadowMap(int w, int h, const glm::vec3& lightPos);

    template <typename T>
    void draw(unsigned int face,
              const T& object,
              const glm::mat4& world);

    void bind(unsigned int face);
    void unbind();

    void setLightPos(const glm::vec3&);

  private:
    int width, height;
    GLuint fb, depth;
    ShaderProgram shader;
    Texture shadowMap;
    glm::mat4 lightProj;
    glm::mat4 viewMats[6];
    glm::vec3 upDirections[6];
    glm::vec3 lightDirections[6];
    glm::vec3 lightPos;
};

template<typename T>
void ShadowMap::draw(unsigned int face,
                     const T& o,
                     const glm::mat4& world) {
  shader.use();
  glm::mat4 wvp = lightProj * viewMats[face] * world;
  shader.add_uniform("WVP", wvp);
  shader.add_uniform("W", world);
  shader.add_uniform("light_pos", lightPos);
  o.draw();
}

}
