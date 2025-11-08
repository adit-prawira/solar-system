#include <engines/graphics/geometry_builder.hpp>

namespace Engines::Graphics {
  // Public 
  auto GeometryBuilder::createCube() -> GeometryBuilder{return {Type::Cube};}
  auto GeometryBuilder::createSphere() -> GeometryBuilder {return {Type::Sphere};}
  auto GeometryBuilder::createSurfaceGrid() -> GeometryBuilder {return {Type::Surface};}
  
  GeometryBuilder& GeometryBuilder::setShader(std::shared_ptr<Engines::Graphics::Shader> shader){
    this->shader = shader;
    return *this;
  }

  GeometryBuilder& GeometryBuilder::setPosition(glm::mat4 position){
    this->position = position;
    return *this;
  }
  
  
  GeometryBuilder& GeometryBuilder::setRadius(float radius){
    this->radius = radius;
    return *this;
  }

  GeometryBuilder& GeometryBuilder::setSize(float size){
    this->size = size;
    return *this;
  }

  GeometryBuilder& GeometryBuilder::setRows(unsigned int rows){
    this->rows = rows;
    return *this;
  }

  GeometryBuilder& GeometryBuilder::setColumns(unsigned int columns){
    this->columns = columns;
    return *this;
  }

  GeometryBuilder& GeometryBuilder::setSpace(float space){
    this->space = space;
    return *this;
  }

  GeometryBuilder& GeometryBuilder::isDebug(bool is_debug){
    this->is_debug = is_debug;
    return *this;
  }

  GeometryBuilder& GeometryBuilder::setColor(glm::vec3 color){
    this->color = color;
    return *this;
  }

  GeometryBuilder& GeometryBuilder::setSectorCount(unsigned int sector_count){
    this->sector_count = sector_count;
    return *this;
  }

  GeometryBuilder& GeometryBuilder::setStackCount(unsigned int stack_count){
    this->stack_count = stack_count;
    return *this;
  }

  std::unique_ptr<Shape> GeometryBuilder::build() const {
    if(this->shader == nullptr) 
      throw std::runtime_error("GeometryBuilder_ERROR ==> Shader is required to render shape");

    switch(type){
      case Type::Sphere:
        return std::make_unique<Sphere>(this->radius, this->color, this->position, 
          this->sector_count, this->stack_count, this->shader, this->is_debug);
      case Type::Cube:
        return std::make_unique<Cube>(this->size, this->color, this->position, 
          this->sector_count, this->stack_count, this->shader, this->is_debug);
      case Type::Surface:
          return std::make_unique<Surface>(this->rows, this->columns, this->space, this->color, this->position, 
          this->sector_count, this->stack_count, this->shader, this->is_debug);
      default:
        return nullptr;
    }
  }

  //Private
  GeometryBuilder::GeometryBuilder(Type t): type(t){};
}