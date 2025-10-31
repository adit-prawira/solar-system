#pragma once
#include <engines/graphics/shape.hpp>

namespace Engines::Graphics {
  class Sphere : public Shape {
    public:
      explicit Sphere(float radius, unsigned int sector_count, unsigned int stack_count, bool is_debug);
      ~Sphere() override;
      void draw() override;
    private:
      GLuint VAO, VBO, EBO;
      float radius = 10.0f;
      unsigned int sector_count = 36;
      unsigned int stack_count = 18;
      std::vector<Vertex> vertices;
      std::vector<unsigned int> indices;

      void processVertices();
      void processIndices();
      void setupMesh();
  };
}