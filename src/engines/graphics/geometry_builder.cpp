#include <engines/graphics/geometry_builder.hpp>

namespace Engines::Graphics {
  // Public 
  auto GeometryBuilder::createCube() -> GeometryBuilder{return {Type::Cube};}
  auto GeometryBuilder::createSphere() -> GeometryBuilder {return {Type::Sphere};}
  
  GeometryBuilder& GeometryBuilder::setRadius(float radius){
    this->radius = radius;
    return *this;
  }

  GeometryBuilder& GeometryBuilder::setSize(float size){
    this->size = size;
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
    switch(type){
      case Type::Sphere:
        return std::make_unique<Sphere>(this->radius, this->sector_count, this->stack_count, this->is_debug);
      case Type::Cube:
        return std::make_unique<Cube>(this->size);
      default:
        return nullptr;
    }
  }

  //Private
  GeometryBuilder::GeometryBuilder(Type t): type(t){};
}