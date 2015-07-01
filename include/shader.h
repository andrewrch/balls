#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>

enum class ShaderType {FRAG = GL_FRAGMENT_SHADER,
                       GEOM = GL_GEOMETRY_SHADER,
                       VERT = GL_VERTEX_SHADER};
class Shader {
  friend class ShaderProgram;
  public:
    explicit Shader(ShaderType type);
    explicit Shader(const std::string& filename, ShaderType type);
    ~Shader();

    void add_source(const std::string& source);
    void add_source_from_file(const std::string& filename);
  private:
    void compile() const;

    std::vector<std::string> shader_source;
    ShaderType type;
    GLuint id;
};
