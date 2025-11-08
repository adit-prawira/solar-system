#include <simulation/flamm_paraboloid.hpp>

namespace Simulation {
  FlammParaboloid& FlammParaboloid::setSurface(std::unique_ptr<Engines::Graphics::Surface> surface){
    this->surface = std::move(surface);
    return *this;
  }

  FlammParaboloid& FlammParaboloid::build(){
    if(!this->surface) this->validation_messages.push_back("VALIDATION: Surface geometry is required");
    if(!this->validation_messages.empty()) std::runtime_error(this->compileValidationMessages());
    return *this;
  }

  Engines::Graphics::Surface* FlammParaboloid::getSurface() const {
    return this->surface.get();
  }

  void FlammParaboloid::apply(std::vector<std::shared_ptr<CelestialBody>> celestial_bodies){
    auto& vertices = this->getSurface()->getVertices();
    const auto& indices = this->getSurface()->getIndices();
    
    for(auto &vertex:vertices)
      vertex.position.y = 0.0f;

    for(auto &celestial_body : celestial_bodies){
      const float mass = celestial_body->getMass();
      const float rs = celestial_body->getRadius();
      const float r_min = rs + std::max(1e-6f, 0.01f * rs); 
      float mass_depth_scale = glm::clamp(mass / Engines::Physics::Constants::MASS_SUN_KG, 0.1f, 1.0f);

      for(auto &vertex : vertices){
        const float x_relative = vertex.position.x - celestial_body->getPosition().x;
        const float z_relative = vertex.position.z - celestial_body->getPosition().z;
        
        float r = std::sqrt(x_relative * x_relative + z_relative*z_relative);   
        if(r < r_min) r = r_min;
        const float pit_depth =  mass_depth_scale * 2.0f * std::sqrt(rs * (r - rs));
        vertex.position.y = std::max(vertex.position.y, pit_depth);
        vertex.position.x = x_relative + celestial_body->getPosition().x;
        vertex.position.z = z_relative + celestial_body->getPosition().z;
        vertex.normal = glm::vec3(0.0f);
      }
    }

    for(size_t i = 0; i + 2 < indices.size(); i+=3){
      auto& v0 = vertices[indices[i]];
      auto& v1 = vertices[indices[i + 1]];
      auto& v2 = vertices[indices[i + 2]];
      glm::vec3 normal = glm::normalize(glm::cross(v1.position - v0.position, v2.position - v0.position));

      v0.normal += normal;
      v1.normal += normal;
      v2.normal += normal;
    }

    for(auto &vertex:vertices)
      vertex.normal = glm::normalize(vertex.normal);
    this->getSurface()->updateVertices();
  }

  std::string FlammParaboloid::compileValidationMessages(){
    std::ostringstream oss;
    for(size_t i = 0; i < this->validation_messages.size(); ++i){
      oss << this->validation_messages[i];
      if(i != this->validation_messages.size() - 1) oss << "\n";
    }
    return oss.str();
  }
}