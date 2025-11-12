#pragma once
#include <memory>
#include <engines/graphics/shape.hpp>
#include <engines/graphics/cube.hpp>
#include <engines/graphics/sphere.hpp>
#include <engines/graphics/surface.hpp>
#include <engines/graphics/ring.hpp>
#include <engines/graphics/shader.hpp>


namespace Engines::Graphics{
  class GeometryBuilder{
    public:
      static GeometryBuilder createSphere();
      static GeometryBuilder createCube();
      static GeometryBuilder createSurfaceGrid();
      static GeometryBuilder createRing();

      GeometryBuilder& setRadius(float radius);

      GeometryBuilder& setSize(float size);

      GeometryBuilder& setOuterRadius(float outer_radius);
      GeometryBuilder& setInnerRadius(float inner_radius);
      GeometryBuilder& setCenter(glm::vec3 center);

      GeometryBuilder& setRows(unsigned int rows);
      GeometryBuilder& setColumns(unsigned int columns);
      GeometryBuilder& setSpace(float space);

      GeometryBuilder& setSectorCount(unsigned int sector_count);
      GeometryBuilder& setStackCount(unsigned int stack_count);
      GeometryBuilder& setColor(glm::vec3 color);
      GeometryBuilder& isDebug(bool is_debug);
      GeometryBuilder& setPosition(glm::mat4 position);
      GeometryBuilder& setShader(std::shared_ptr<Engines::Graphics::Shader> shader);
      std::unique_ptr<Shape> build() const;

    private:
      float radius = 10.0f;
      unsigned int sector_count = 30;
      unsigned int stack_count = 30;
      float outer_radius = 30.0f;
      float inner_radius = 10.0f;
      unsigned int segments = 128;
      glm::vec3 center {0.0f, 0.0f, 0.0f};
      float size = 10.0f;
      unsigned int rows = 10;
      unsigned int columns = 10;
      float space = 10.0f;
      bool is_debug = false;
      std::shared_ptr<Engines::Graphics::Shader> shader = nullptr;
      glm::vec3 color{0.6f, 1.0f, 1.0f};
      glm::mat4 position;
      enum class Type {None, Cube, Sphere, Surface, Ring};
      Type type = Type::None;

      // Private constructor
      GeometryBuilder(Type t); 
  };
}