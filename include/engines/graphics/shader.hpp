#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>

#include <sstream>
#include <fstream>
#include <iostream>

namespace Engines::Graphics {
  class Shader {
    public:
      GLuint ID;
      Shader(const char *vertex_path, const char *fragment_path);

      void use() const {glUseProgram(ID);}

      void setVec3(const std::string &name, const glm::vec3 &value) const;
      void setMat4(const std::string &name, const glm::mat4 &value) const;
      void setBool(const std::string &name, bool value) const;
      void setInt(const std::string &name, int value) const;
      void setFloat(const std::string &name, float value) const;
  };
}