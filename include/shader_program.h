#pragma once

#include <vector>
#include "shader.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram {
  public:
    explicit ShaderProgram(const std::vector<Shader>& shaders);;
    ~ShaderProgram();

    void use();

    template <typename T>
    void add_uniform(const std::string& name, T uniform);
    //template <typename T>
    //void add_uniform(const std::string& name, const T& uniform);

    //void add_attribute();
  private:
    GLenum id;
};

