#include <simulation/mars.hpp>

namespace Simulation{
  std::shared_ptr<Simulation::CelestialBody> Mars::build(std::shared_ptr<Engines::Graphics::Shader> shader){
    return this->buildBase(
      Engines::Physics::Constants::RADIUS_MARS_M,
      Engines::Physics::Constants::MASS_MARS_KG,
      shader
    );
  }
}