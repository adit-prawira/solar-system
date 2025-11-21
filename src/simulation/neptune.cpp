#include <simulation/neptune.hpp>

namespace Simulation{
  std::shared_ptr<Simulation::CelestialBody> Neptune::build(std::shared_ptr<Engines::Graphics::Shader> shader){
    return this->buildBase(
      "Neptune",
      Engines::Physics::Constants::RADIUS_NEPTUNE_M,
      Engines::Physics::Constants::MASS_NEPTUNE_KG,
      shader
    );
  }
}