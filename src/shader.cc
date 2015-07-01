#include "shader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

Shader::Shader(ShaderType t) : shader_source(0), type(t) {
  id = glCreateShader(static_cast<int>(type));
}

Shader::Shader(const std::string& filename, ShaderType t) : type(t) {
  id = glCreateShader(static_cast<int>(type));
  add_source_from_file(filename);
}

Shader::~Shader() {
//  glDeleteShader(id);
}

void Shader::add_source(const std::string& source) {
  shader_source.push_back(source);
}

void Shader::add_source_from_file(const std::string& filename) {
  std::ifstream file(filename);
  std::string source((std::istreambuf_iterator<char>(file)),
                      std::istreambuf_iterator<char>());
  shader_source.push_back(source);
}

void Shader::compile() const {
  const char* source_text[shader_source.size()];
  for (unsigned int i = 0; i < shader_source.size(); i++) {
    source_text[i] = shader_source[i].c_str();
  }
  glShaderSource(id, shader_source.size(), source_text, nullptr);
  //check whether the shader loads fine
  GLint status;
  glCompileShader(id);
  glGetShaderiv(id, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE) {
    GLint infoLogLength;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar infoLog[infoLogLength];
    glGetShaderInfoLog (id, infoLogLength, NULL, infoLog);
    std::stringstream err;
    err << "Shader compilation failure" << std::endl;
    err << infoLog << std::endl;
    throw std::runtime_error(err.str());
  }
}
