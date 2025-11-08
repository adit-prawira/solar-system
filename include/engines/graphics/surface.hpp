#pragma once 
#include <engines/graphics/shape.hpp>

namespace Engines::Graphics {
  class Surface : public Shape {
    public:
      explicit Surface(
        unsigned int rows, 
        unsigned int columns,
        float space, 
        glm::vec3 color, 
        glm::mat4 position,
        unsigned int sector_count, 
        unsigned int stack_count, 
        std::shared_ptr<Engines::Graphics::Shader> shader,
        bool is_debug);
      ~Surface() override;
      void draw() override;

      float getSpace() const;
      unsigned int getRows() const;
      unsigned int getColumns() const;
    private:
      unsigned int rows = 10;
      unsigned int columns = 10;
      float space = 10.0f;
      void processVertices() override;
      void processIndices() override;
      void setupMesh() override;
  };
}