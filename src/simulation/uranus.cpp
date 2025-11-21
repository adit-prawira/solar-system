#include <simulation/uranus.hpp>

namespace Simulation{
  std::shared_ptr<Simulation::CelestialBody> Uranus::build(std::shared_ptr<Engines::Graphics::Shader> shader){
    return this->buildBase(
      "Uranus",
      Engines::Physics::Constants::RADIUS_URANUS_M,
      Engines::Physics::Constants::MASS_URANUS_KG,
      shader
    );
  }
}