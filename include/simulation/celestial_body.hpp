#pragma once
#include <engines/physics/physics_body.hpp>
#include <engines/graphics/geometry_builder.hpp>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace Simulation {
  class CelestialBody : public Engines::Physics::PhysicsBody<CelestialBody> {
    public:
      CelestialBody& setRadius(float radius){
        this->radius = radius;
        return *this;
      }
      CelestialBody& setHasOrbit(float has_orbit){
        this->has_orbit = has_orbit;
        return *this;
      }
      
      CelestialBody& setOrbitCenter(glm::vec3 orbit_center){
        this->orbit_center = orbit_center;
        return *this;
      }

      CelestialBody& setOrbitColor(glm::vec3 orbit_color){
        this->orbit_color = orbit_color;
        return *this;
      }
      
      CelestialBody& setOrbitShader(std::shared_ptr<Engines::Graphics::Shader> orbit_shader){
        this->orbit_shader = orbit_shader;
        return *this;
      }

      auto hasOrbit() -> bool {return has_orbit;}
      auto getOrbit() -> std::shared_ptr<Engines::Graphics::Shape>{return orbit;}

      CelestialBody& build() override {
        this->executeValidation([this](){
          if(!this->radius) this->validation_messages.push_back("Radius is required for celestial body");
        });
        if(this->has_orbit && this->orbit_shader){
          auto ring_position = glm::translate(glm::mat4(1.0f), this->orbit_center);
          this->orbit = Engines::Graphics::GeometryBuilder::createRing()
          .setOuterRadius(position.x)
          .setInnerRadius(position.x - 3.0f)
          .setCenter(glm::vec3(0.0f, 0.0f, 0.0f))
          .setPosition(ring_position)
          .setColor(this->orbit_color)
          .setShader(this->orbit_shader)
          .build();
        }
        return *this; 
      };
      float getRadius(){return radius;};

    private:
      float radius;
      bool has_orbit = false;
      std::shared_ptr<Engines::Graphics::Shader> orbit_shader;
      glm::vec3 orbit_center{0.0f, 0.0f, 0.0f};
      glm::vec3 orbit_color{0.0f, 0.0f, 0.0f};

      std::shared_ptr<Engines::Graphics::Shape> orbit;
      
  };
}