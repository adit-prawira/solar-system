#pragma once
#include <simulation/builder/celestial_body_builder.hpp>

namespace Simulation {
  class Mercury : public CelestialBodyBuilder {
    public:
      std::shared_ptr<Simulation::CelestialBody> build(std::shared_ptr<Engines::Graphics::Shader> shader);
  };
}