#include <engines/graphics/sphere.hpp>
#include <iostream>

namespace Engines::Graphics {
  // Public
  Sphere::Sphere(
        float radius, 
        glm::vec3 color, 
        glm::mat4 position,
        unsigned int sector_count, 
        unsigned int stack_count, 
        std::shared_ptr<Engines::Graphics::Shader> shader,
        bool is_debug):
        Shape(color, position, sector_count, stack_count, shader, is_debug),
        radius(radius){
    this->processVertices();
    this->processIndices();
    this->setupMesh();
  }

  Sphere::~Sphere(){
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
  }

  void Sphere::draw() {
    this->shader->setMat4("model", this->position);
    this->shader->setVec3("object_color", this->color);

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

  // Private
  void Sphere::processVertices(){
    for(unsigned int i = 0; i <= this->stack_count; ++i){
      const float phi = M_PI/2.0f - M_PI*(static_cast<float>(i) / this->stack_count);
      const float z = this->radius*sinf(phi);

      for(unsigned int j = 0; j <= this->sector_count; ++j){
        const float theta = 2.0f * M_PI * (static_cast<float>(j)/ this->sector_count);
        const float x = this->radius*cosf(phi)*cosf(theta);
        const float y = this->radius*cosf(phi)*sinf(theta);
        
        const glm::vec3 position(x,y,z);
        const glm::vec3 normal = position/this->radius;
        const glm::vec2 texture(
          static_cast<float>(j)/this->sector_count, 
          static_cast<float>(i)/this->stack_count);
        this->vertices.push_back({position, normal, texture});
      }
    }
    if(!this->is_debug) return;
    
    for(auto vertex:vertices){
      std::cout << "Position(x, y, z) = " << "(" << vertex.position.x << ", " << vertex.position.y << ", " << vertex.position.z << ")" << std::endl;
      std::cout << "Normal(x, y, z) = " << "(" << vertex.normal.x << ", " << vertex.normal.y << ", " << vertex.normal.z << ")" << std::endl;
      std::cout << "Texture(x, y) = " << "(" << vertex.texture.x << ", " << vertex.texture.y <<  ")" << std::endl;
    }
  }

  void Sphere::processIndices(){
    for(unsigned int i = 0; i < this->stack_count; ++i){
      unsigned int k1 = i * (this->sector_count + 1);
      unsigned int k2 = k1 + this->sector_count + 1;
      for(unsigned j =0; j < this->sector_count; ++j, ++k1, ++k2){
        const bool is_top_sphere = i == 0;
        const bool is_bottom_sphere = i == this->stack_count - 1;
        
        // Create first triangle => k1 -> k2 -> k1+1
        // Skip process if located at the north pole of the sphere
        if(!is_top_sphere) 
          this->indices.insert(this->indices.end(), {k1, k2, k1+1});
        
        // Create second triangle => k1+1 -> k2 -> k2+1
        // Skip process of located at the south pole of the sphere
        if(!is_bottom_sphere) 
          this->indices.insert(this->indices.end(), {k1+1, k2, k2+1});
      }
    }
  }

  void Sphere::setupMesh(){
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    // Texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
  }
}