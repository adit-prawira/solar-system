#include <simulation/saturn.hpp>

namespace Simulation{
  std::shared_ptr<Simulation::CelestialBody> Saturn::build(std::shared_ptr<Engines::Graphics::Shader> shader){
    return this->buildBase(
      "Saturn",
      Engines::Physics::Constants::RADIUS_SATURN_M,
      Engines::Physics::Constants::MASS_SATURN_KG,
      shader
    );
  }
}