#include <engines/graphics/surface.hpp>

namespace Engines::Graphics {
  Surface::Surface(
        unsigned int rows, 
        unsigned int columns,
        float space, 
        glm::vec3 color, 
        glm::mat4 position,
        unsigned int sector_count, 
        unsigned int stack_count, 
        std::shared_ptr<Engines::Graphics::Shader> shader,
        bool is_debug): 
          Shape(color, position, sector_count, stack_count, shader, is_debug), 
          rows(rows), 
          columns(columns),
          space(space){
            this->processVertices();
            this->processIndices();
            this->setupMesh();
          }
  
  
  // Clean up
  Surface::~Surface(){
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
  }

  auto Surface::getSpace() const -> float {return space;}
  auto Surface::getRows() const -> unsigned int {return rows;}
  auto Surface::getColumns() const -> unsigned int {return columns;}
  
  void Surface::draw(){
    // call shader
    this->shader->setMat4("model", this->position);
    this->shader->setVec3("object_color", this->color);

    // bind and draw vertices
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
  
  void Surface::processVertices(){
    const float half_width = ((this->rows - 1)* this->space)/2.0f;
    const float half_height = ((this->columns - 1) * this->space)/2.0f;

    for(unsigned int i = 0; i < this->rows; ++i){
      for(unsigned int j = 0; j < this->columns; ++j){
        float x = (i*this->space) - half_width;
        float z = (j*this->space) - half_height;
        this->vertices.push_back({
          {x, 0.0f, z}, // position
          {0.0f, 1.0f, 0.0f}, // normal
          {j/static_cast<float>(this->columns-1), i/static_cast<float>(this->rows-1)} // texture
        });
      }
    }
    if(this->is_debug) this->logVertices();
  }

  void Surface::processIndices(){
    for(unsigned int i = 0; i < this->rows-1; ++i){
      for(unsigned int j = 0; j < this->columns-1; ++j){
        unsigned int top_left = i*this->columns + j;
        unsigned int top_right = top_left + 1;
        unsigned int bottom_left = (i+1) * this->columns + j;
        unsigned int bottom_right = bottom_left + 1;

        // First triangle
        this->indices.insert(this->indices.end(), {top_left, top_right, bottom_left});

        // Second triangle
        this->indices.insert(this->indices.end(), {top_right, bottom_left, bottom_right});
      }
    }
  }
  
  void Surface::setupMesh(){
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), this->vertices.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
  }
}