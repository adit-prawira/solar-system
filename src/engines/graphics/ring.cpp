#include <engines/graphics/ring.hpp>

namespace Engines::Graphics {
  Ring::Ring(
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
  ): Shape(color, position, sector_count, stack_count, shader, is_debug),
    outer_radius(outer_radius),
    inner_radius(inner_radius),
    segments(segments),
    center(center){
      this->processVertices();
      this->processIndices();
      this->setupMesh();
    }

  Ring::~Ring(){
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
  }
  
  void Ring::draw(){
    this->shader->setMat4("model", this->position);
    this->shader->setVec3("object_color", this->color);

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

  void Ring::processVertices(){
    for(unsigned int i = 0; i < this->segments; ++i){
      const float theta = 2*M_PI*i/this->segments;
      
      Vertex outer_vertex;
      outer_vertex.position = this->center 
        + glm::vec3(
          this->outer_radius*cos(theta), 
          0.0f, 
          this->outer_radius*sin(theta));
      outer_vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
      outer_vertex.texture = glm::vec2(static_cast<float>(i)/this->segments, 1.0f);   
      Vertex inner_vertex;
      inner_vertex.position = this->center + glm::vec3(this->inner_radius*cos(theta), 0.0f, this->inner_radius*sin(theta));
      inner_vertex.normal = glm::vec3(0.0f, 1.0f, 0.0f);
      inner_vertex.texture = glm::vec2(static_cast<float>(i)/this->segments, 1.0f);

      this->vertices.insert(this->vertices.end(), {outer_vertex, inner_vertex});
    }
  }

  void Ring::processIndices(){
    for(unsigned int i = 0; i < this->segments; ++i){
      const int next = (i+1) % this->segments;
      unsigned int outer_current = i*2;
      unsigned int outer_next = next*2;

      unsigned int inner_current = i*2 + 1;
      unsigned int inner_next = next*2 + 1;

      this->indices.insert(this->indices.end(), {outer_current, inner_current, outer_next});
      this->indices.insert(this->indices.end(), {inner_current, inner_next, outer_next});
    }
  }

  void Ring::setupMesh(){
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size()*sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
  }
}