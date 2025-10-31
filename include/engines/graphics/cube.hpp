#pragma once
#include <engines/graphics/shape.hpp>

namespace Engines::Graphics {
  class Cube : public Shape {
    public:
      explicit Cube(float size = 10.0f);
      ~Cube() override = default;
      void draw() override;
    private:
      float size = 10.0f;
  };
}