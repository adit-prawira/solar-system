#include <engines/graphics/cube.hpp>
#include <iostream>

namespace Engines::Graphics {
  Cube::Cube(float size, 
        glm::vec3 color, 
        glm::mat4 position,
        unsigned int sector_count, 
        unsigned int stack_count, 
        std::shared_ptr<Engines::Graphics::Shader> shader,
        bool is_debug):
        Shape(color, position, sector_count, stack_count, shader, is_debug),
        size(size){};

  Cube::~Cube(){
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
  }
  
  void Cube::draw() {
    std::cout << "Drawing Cube -> " << this->size << std::endl;
  }

  void Cube::processVertices(){
    std::cout << "Process vertices for cube" << std::endl;
  }

  void Cube::processIndices(){
    std::cout << "Process indices for cube" << std::endl;
  }

  void Cube::setupMesh(){
    std::cout << "Setup mesh for cube" << std::endl;
  }
}