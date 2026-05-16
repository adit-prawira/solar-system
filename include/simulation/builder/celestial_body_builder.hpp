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
      CelestialBodyBuilder& setIsDebugMode(bool is_debug_mode);
      CelestialBodyBuilder& setOrbitalVelocity(glm::vec3 orbital_velocity);
      CelestialBodyBuilder& setRenderRadiusMagnification(float magnification);
      CelestialBodyBuilder& setRenderPositionMagnification(float magnification);
      
      CelestialBodyBuilder& setHasOrbit(bool has_orbit);
      CelestialBodyBuilder& setIsStar(float is_star);
      CelestialBodyBuilder& setIsShowTrail(bool is_show_trail); 
      CelestialBodyBuilder& setOrbitCenter(glm::dvec3 orbit_center);

      virtual std::shared_ptr<Simulation::CelestialBody> build(
        std::shared_ptr<Engines::Graphics::Shader> shader) = 0;
    protected:
      bool has_orbit = false;
      float is_star = false;
      bool is_show_trail = false;
      bool is_debug_mode = false;

      glm::vec3 position{0.0f, 50.0f, 0.0f};
      glm::vec3 orbital_velocity{0.0f, 50.0f, 0.0f};

      glm::vec3 color{1.0f, 0.3f, 0.0f};
      glm::dvec3 orbit_center{0.0f, 0.0f, 0.0f};
      
      float render_radius_magnification = 1.0f;
      float render_position_magnification = 1.0f;

      std::shared_ptr<Simulation::CelestialBody> buildBase(
        const std::string name,
        float radius_m, 
        float mass_kg,
        std::shared_ptr<Engines::Graphics::Shader> shader);

  };
}
