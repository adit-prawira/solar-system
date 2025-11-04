#pragma once
#include <engines/graphics/shape.hpp>

namespace Engines::Graphics {
  class Cube : public Shape {
    public:
      explicit Cube(
        float size, 
        glm::vec3 color,
        glm::mat4 position,
        unsigned int sector_count, 
        unsigned int stack_count,
        std::shared_ptr<Engines::Graphics::Shader> shader,
        bool is_debug
      );
      ~Cube() override;
      void draw() override;
    private:
      float size = 10.0f;
      void processVertices() override;
      void processIndices() override;
      void setupMesh() override;
  };
}