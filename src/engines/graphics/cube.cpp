#include <engines/graphics/cube.hpp>
#include <iostream>

namespace Engines::Graphics {
  Cube::Cube(float size):size(size){};

  void Cube::draw() {
    std::cout << "Drawing Cube -> " << this->size << std::endl;
  }
}