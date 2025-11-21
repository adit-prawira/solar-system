#include <simulation/venus.hpp>

namespace Simulation{
  std::shared_ptr<Simulation::CelestialBody> Venus::build(std::shared_ptr<Engines::Graphics::Shader> shader){
    return this->buildBase(
      "Venus",
      Engines::Physics::Constants::RADIUS_VENUS_M,
      Engines::Physics::Constants::MASS_VENUS_KG,
      shader
    );
  }
}