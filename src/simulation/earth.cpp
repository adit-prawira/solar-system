#include <simulation/earth.hpp>

namespace Simulation{
  std::shared_ptr<Simulation::CelestialBody> Earth::build(std::shared_ptr<Engines::Graphics::Shader> shader){
    return this->buildBase(
      Engines::Physics::Constants::RADIUS_EARTH_M,
      Engines::Physics::Constants::MASS_EARTH_KG,
      shader
    );
  }
}