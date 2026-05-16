#include <simulation/builder/celestial_body_builder.hpp>

namespace Simulation{
  CelestialBodyBuilder& CelestialBodyBuilder::setPosition(glm::vec3 position){
    this->position = position;
    return *this;
  }
  CelestialBodyBuilder& CelestialBodyBuilder::setColor(glm::vec3 color){
    this->color = color;
    return *this;
  }

  CelestialBodyBuilder& CelestialBodyBuilder::setOrbitalVelocity(glm::vec3 orbital_velocity){
    this->orbital_velocity = orbital_velocity;
    return *this;
  }

  CelestialBodyBuilder& CelestialBodyBuilder::setHasOrbit(bool has_orbit){
    this->has_orbit = has_orbit;
    return *this;
  }

  CelestialBodyBuilder& CelestialBodyBuilder::setIsDebugMode(bool is_debug_mode){
    this->is_debug_mode = is_debug_mode;
    return *this;
  }
  
  CelestialBodyBuilder& CelestialBodyBuilder::setIsStar(float is_star){
    this->is_star = is_star;
    return *this;
  }

  CelestialBodyBuilder& CelestialBodyBuilder::setIsShowTrail(bool is_show_trail) {
    this->is_show_trail = is_show_trail;
    return *this;
  }
  CelestialBodyBuilder& CelestialBodyBuilder::setOrbitCenter(glm::dvec3 orbit_center){
    this->orbit_center = orbit_center;
    return *this;
  }

  CelestialBodyBuilder& CelestialBodyBuilder::setRenderRadiusMagnification(float magnification){
    this->render_radius_magnification = magnification;
    return *this;
  }

  CelestialBodyBuilder& CelestialBodyBuilder::setRenderPositionMagnification(float magnification){
    this->render_position_magnification = magnification;
    return *this;
  }

  std::shared_ptr<Simulation::CelestialBody> CelestialBodyBuilder::buildBase(
    const std::string name,
    float radius_m,
    float mass_kg,
    std::shared_ptr<Engines::Graphics::Shader> shader){
    auto celestial_body = std::make_shared<Simulation::CelestialBody>();
    celestial_body->setName(name)
      .setIsDebugMode(this->is_debug_mode)
      .setIsStar(this->is_star)
      .setRadius(radius_m)
      .setMass(mass_kg)
      .setPosition(this->position)
      .setRenderRadiusMagnification(this->render_radius_magnification)
      .setRenderPositionMagnification(this->render_position_magnification)
      .setVelocity(glm::vec3{0.0f, 0.0f, 0.0f})
      .setAcceleration(glm::vec3{0.0f, 0.0f, 0.0f})
      .setHasOrbit(this->has_orbit)
      .setOrbitCenter(this->orbit_center)
      .setOrbitColor(this->color)
      .setOrbitShader(shader)
      .setTrailShader(shader)
      .setIsShowTrail(this->is_show_trail);
    
    auto model_matrix = glm::translate(glm::mat4(1.0f), celestial_body->getRenderPosition());
    auto shape = Engines::Graphics::GeometryBuilder::createSphere()
      .setRadius(celestial_body->getRenderRadius()) 
      .setSectorCount(30)
      .setStackCount(18)
      .setModelMatrix(model_matrix)
      .setColor(this->color)
      .setShader(shader)
      .build();
    celestial_body->setShape(std::move(shape)).build();
    return celestial_body;
  }
}
