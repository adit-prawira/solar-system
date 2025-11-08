#pragma once
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <engines/graphics/shader.hpp>

namespace Engines::Graphics {
  struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture;
  };
  class Shape {
    public:
      Shape(
        glm::vec3 color,
        glm::mat4 position,
        unsigned int sector_count, 
        unsigned int stack_count, 
        std::shared_ptr<Engines::Graphics::Shader> shader,
        bool is_debug);

      virtual ~Shape() = default;
      virtual void draw() = 0;
      
      std::vector<Vertex>& getVertices();
      std::vector<unsigned int>& getIndices();

      void updateVertices();
    protected:
      GLuint VAO, VBO, EBO;

      bool is_debug = false;
      unsigned int sector_count = 36;
      unsigned int stack_count = 18;
      std::shared_ptr<Engines::Graphics::Shader> shader = nullptr;
      glm::vec3 color{0.6f, 1.0f, 1.0f};
      glm::mat4 position;
      std::vector<Vertex> vertices;
      std::vector<unsigned int> indices;

      virtual void processVertices() = 0;
      virtual void processIndices() = 0;
      virtual void setupMesh() = 0;

      void logVertices();
  };
}