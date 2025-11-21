#include <simulation/jupiter.hpp>

namespace Simulation{
  std::shared_ptr<Simulation::CelestialBody> Jupiter::build(std::shared_ptr<Engines::Graphics::Shader> shader){
    return this->buildBase(
      "Jupiter",
      Engines::Physics::Constants::RADIUS_JUPITER_M,
      Engines::Physics::Constants::MASS_JUPITER_KG,
      shader
    );
  }
}