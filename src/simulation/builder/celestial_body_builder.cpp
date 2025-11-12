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

  CelestialBodyBuilder& CelestialBodyBuilder::setHasOrbit(float has_orbit){
    this->has_orbit = has_orbit;
    return *this;
  }

  CelestialBodyBuilder& CelestialBodyBuilder::setOrbitCenter(glm::vec3 orbit_center){
    this->orbit_center = orbit_center;
    return *this;
  }

  CelestialBodyBuilder& CelestialBodyBuilder::setMagnification(float magnification){
    this->magnification = magnification;
    return *this;
  }

  glm::mat4 CelestialBodyBuilder::getRenderPosition(){
    return glm::translate(glm::mat4(1.0f), this->position);
  }

  std::shared_ptr<Simulation::CelestialBody> CelestialBodyBuilder::buildBase(
    float radius_m,
    float mass_kg,
    std::shared_ptr<Engines::Graphics::Shader> shader){
    auto radius_px = Engines::Maths::Converter::meterToPixel(radius_m, &this->magnification);
    auto shape = Engines::Graphics::GeometryBuilder::createSphere()
            .setRadius(radius_px) 
            .setSectorCount(30)
            .setStackCount(18)
            .setPosition(this->getRenderPosition())
            .setColor(this->color)
            .setShader(shader)
            .build();
    auto celestial_body = std::make_shared<Simulation::CelestialBody>();
    celestial_body->setRadius(radius_px)
      .setMass(mass_kg)
      .setPosition(this->position)
      .setVelocity(glm::vec3{0.0f, 0.0f, 0.0f})
      .setAcceleration(glm::vec3{0.0f, 0.0f, 0.0f})
      .setShape(std::move(shape))
      .setHasOrbit(this->has_orbit)
      .setOrbitCenter(this->orbit_center)
      .setOrbitColor(this->color)
      .setOrbitShader(shader)
      .build();
    return celestial_body;
  }
}