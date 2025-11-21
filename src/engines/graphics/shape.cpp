#include <engines/graphics/shape.hpp>

namespace Engines::Graphics{
  Shape::Shape(
        glm::vec3 color,
        glm::mat4 model_matrix,
        unsigned int sector_count, 
        unsigned int stack_count, 
        std::shared_ptr<Engines::Graphics::Shader> shader,
        bool is_debug)
        {
          this->color = color;
          this->model_matrix = model_matrix;
          this->sector_count = sector_count;
          this->stack_count = stack_count;
          this->is_debug = is_debug;
          this->shader = shader;
        };
  
  auto Shape::getVertices() -> std::vector<Vertex>&{ return vertices;}
  auto Shape::getIndices() -> std::vector<unsigned int>&{ return indices;}
  auto Shape::getModelMatrix() -> glm::mat4{return model_matrix;}
  auto Shape::getColor() -> glm::vec3{return color;}
  void Shape::updateModelMatrix(glm::mat4 new_model_matrix){
    this->model_matrix = new_model_matrix;
  }
  void Shape::updateVertices(){
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, this->vertices.size() * sizeof(Vertex), this->vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void Shape::logVertices(){
    for(auto vertex:vertices){
      std::cout << "Position(x, y, z) = " << "(" << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z << ")" << std::endl;
      std::cout << "Normal(x, y, z) = " << "(" << vertex.normal.x << ", " << vertex.normal.y << ", " << vertex.normal.z << ")" << std::endl;
      std::cout << "Texture(x, y) = " << "(" << vertex.texture.x << ", " << vertex.texture.y <<  ")" << std::endl;
    } 
  }
}