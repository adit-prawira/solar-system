#pragma once
#include <engines/physics/constants.hpp>
#include <engines/graphics/surface.hpp>
#include <simulation/celestial_body.hpp>
#include <memory>

namespace Simulation {
  class FlammParaboloid {
    public:
      FlammParaboloid& setSurface(std::unique_ptr<Engines::Graphics::Surface> surface);
      FlammParaboloid& build();

      Engines::Graphics::Surface* getSurface() const;
      void apply(std::vector<std::shared_ptr<CelestialBody>> celestial_bodies);
    private:
      std::vector<std::string> validation_messages;
      std::unique_ptr<Engines::Graphics::Surface> surface;
    std::string compileValidationMessages();
  };
}