#include <memory>
#include <engines/graphics/shape.hpp>
#include <engines/graphics/cube.hpp>
#include <engines/graphics/sphere.hpp>
#include <engines/graphics/shader.hpp>


namespace Engines::Graphics{
  class GeometryBuilder{
    public:
      static GeometryBuilder createSphere();
      static GeometryBuilder createCube();

      GeometryBuilder& setRadius(float radius);
      GeometryBuilder& setSectorCount(unsigned int sector_count);
      GeometryBuilder& setStackCount(unsigned int stack_count);
      GeometryBuilder& setColor(glm::vec3 color);
      GeometryBuilder& isDebug(bool is_debug);
      GeometryBuilder& setSize(float size);
      GeometryBuilder& setPosition(glm::mat4 position);
      GeometryBuilder& setShader(std::shared_ptr<Engines::Graphics::Shader> shader);
      std::unique_ptr<Shape> build() const;

    private:
      float radius = 10.0f;
      unsigned int sector_count = 30;
      unsigned int stack_count = 30;
      float size = 10.0f;
      bool is_debug = false;
      std::shared_ptr<Engines::Graphics::Shader> shader = nullptr;
      glm::vec3 color{0.6f, 1.0f, 1.0f};
      glm::mat4 position;
      enum class Type {None, Cube, Sphere};
      Type type = Type::None;

      // Private constructor
      GeometryBuilder(Type t); 
  };
}