#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <simulation/celestial_body.hpp>
#include <engines/graphics/geometry_builder.hpp>
#include <engines/maths/converter.hpp>
#include <memory>

namespace Simulation {
  class CelestialBodyBuilder {
    public:
      CelestialBodyBuilder& setPosition(glm::vec3);
      CelestialBodyBuilder& setColor(glm::vec3 color);
      CelestialBodyBuilder& setMagnification(float magnification);
      glm::mat4 getRenderPosition();
      virtual std::shared_ptr<Simulation::CelestialBody> build(
        std::shared_ptr<Engines::Graphics::Shader> shader) = 0;
    protected:
      glm::vec3 position{0.0f, 50.0f, 0.0f};
      glm::vec3 color{1.0f, 0.3f, 0.0f};
      float magnification = 1.0f;
      std::shared_ptr<Simulation::CelestialBody> buildBase(
        float radius_m, 
        float mass_kg,
        std::shared_ptr<Engines::Graphics::Shader> shader);

  };
}