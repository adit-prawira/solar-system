#include <simulation/sun.hpp>

namespace Simulation{
  std::shared_ptr<Simulation::CelestialBody> Sun::build(std::shared_ptr<Engines::Graphics::Shader> shader){
    return this->buildBase(
      "Sun",
      Engines::Physics::Constants::RADIUS_SUN_M,
      Engines::Physics::Constants::MASS_SUN_KG,
      shader
    );
  }
}