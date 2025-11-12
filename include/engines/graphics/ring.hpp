#pragma once
#include <engines/graphics/shape.hpp>

namespace Engines::Graphics {
  class Ring : public Shape {
    public:
      explicit Ring(
        float outer_radius,
        float inner_radius,
        unsigned int segments,
        glm::vec3 center,
        glm::vec3 color, 
        glm::mat4 position,
        unsigned int sector_count, 
        unsigned int stack_count, 
        std::shared_ptr<Engines::Graphics::Shader> shader,
        bool is_debug
      );

      ~Ring() override;
      void draw() override;
      
    private:
      float outer_radius = 20.0f;
      float inner_radius = 15.0f;
      unsigned int segments = 200;
      glm::vec3 center{0.0f, 0.0f, 0.0f};
      void processVertices() override;
      void processIndices() override;
      void setupMesh() override;
  };
}