#pragma once
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <glad/glad.h>

namespace Engines::Graphics {
  class Shape {
    public:
      virtual ~Shape() = default;
      virtual void draw() = 0;

    protected:
      bool is_debug = false;
      glm::vec3 color{0.6f, 1.0f, 1.0f};
  };

  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture;
  };
}