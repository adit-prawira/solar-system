#include <simulation/pluto.hpp>

namespace Simulation{
  std::shared_ptr<Simulation::CelestialBody> Pluto::build(std::shared_ptr<Engines::Graphics::Shader> shader){
    return this->buildBase(
      Engines::Physics::Constants::RADIUS_PLUTO_M,
      Engines::Physics::Constants::MASS_PLUTO_KG,
      shader
    );
  }
}