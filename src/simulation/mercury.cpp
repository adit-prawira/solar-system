#include <simulation/mercury.hpp>

namespace Simulation{
  std::shared_ptr<Simulation::CelestialBody> Mercury::build(std::shared_ptr<Engines::Graphics::Shader> shader){
    return this->buildBase(
      Engines::Physics::Constants::RADIUS_MERCURY_M,
      Engines::Physics::Constants::MASS_MERCURY_KG,
      shader
    );
  }
}