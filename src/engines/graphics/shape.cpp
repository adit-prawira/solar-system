#include <engines/graphics/shape.hpp>

namespace Engines::Graphics{
  Shape::Shape(
        glm::vec3 color,
        glm::mat4 position,
        unsigned int sector_count, 
        unsigned int stack_count, 
        std::shared_ptr<Engines::Graphics::Shader> shader,
        bool is_debug)
        {
          this->color = color;
          this->position = position;
          this->sector_count = sector_count;
          this->stack_count = stack_count;
          this->is_debug = is_debug;
          this->shader = shader;
        };

  void Shape::logVertices(){
    for(auto vertex:vertices){
      std::cout << "Position(x, y, z) = " << "(" << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z << ")" << std::endl;
      std::cout << "Normal(x, y, z) = " << "(" << vertex.normal.x << ", " << vertex.normal.y << ", " << vertex.normal.z << ")" << std::endl;
      std::cout << "Texture(x, y) = " << "(" << vertex.texture.x << ", " << vertex.texture.y <<  ")" << std::endl;
    } 
  }
}