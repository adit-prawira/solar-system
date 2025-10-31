#include <engines/graphics/shader.hpp>

namespace Engines::Graphics {
  // Public
  Shader::Shader(const char *vertex_path, const char *fragment_path){
    std::ifstream vertex_file(vertex_path), fragment_file(fragment_path);
    std::stringstream vertex_stream, fragment_stream;

    vertex_stream << vertex_file.rdbuf();
    fragment_stream << fragment_file.rdbuf();

    const std::string vertex_code = vertex_stream.str(), fragment_code = fragment_stream.str();
    const char* vertex_shader_code = vertex_code.c_str();
    const char* fragment_shader_code = fragment_code.c_str();

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_shader_code, nullptr);
    glCompileShader(vertex);

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_shader_code, nullptr);
    glCompileShader(fragment);

    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex);
    glAttachShader(this->ID, fragment);
    glLinkProgram(this->ID);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }

  // Private
  void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int) value);
  }

  void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
  }

  void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
  }

  void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(this->ID, name.c_str()), 1, &value[0]);
  }

  void Shader::setMat4(const std::string &name, const glm::mat4 &value) const {
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
  }
}