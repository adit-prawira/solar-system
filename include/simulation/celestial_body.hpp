#pragma once
#include <engines/physics/physics_body.hpp>
#include <engines/physics/constants.hpp>
#include <engines/graphics/geometry_builder.hpp>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace Simulation {
  class CelestialBody : public Engines::Physics::PhysicsBody<CelestialBody> {
    public:
      CelestialBody& setName(std::string name){
        this->name = name;
        return *this;
      }
      CelestialBody& setIsStar(bool is_star){
        this->is_star = is_star;
        return *this;
      }

      CelestialBody& setIsDebugMode(bool is_debug_mode){
        this->is_debug_mode = is_debug_mode;
        return *this;
      }

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
  
      CelestialBody& setOrbitalVelocity(glm::vec3 orbital_velocity){
        this->velocity = orbital_velocity;
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
      auto getName() -> std::string {return name;}
      auto isStar() -> bool {return is_star;}
      auto isDebugMode() -> bool {return is_debug_mode;}

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

      void move() override {
        std::cout << "Celestial body moving" << std::endl;
      }

      void revolve(std::vector<std::shared_ptr<CelestialBody>> celestial_bodies, const float dt){
        this->acceleration = glm::vec3(0.0f);
        
        for(auto &celestial_body : celestial_bodies){
          if(this->isStar()) continue;
          if(this->getName() == celestial_body->getName()) continue;
          
          const glm::vec3 r = celestial_body->getPosition() - this->getPosition();
          const float distance = glm::length(r);
          
          if(distance <= 0) continue;

          // unit vector determines direction r^
          const glm::vec3 direction = glm::normalize(r);
          this->acceleration += ((Engines::Physics::Constants::GRAVITATIONAL_CONSTANT  * celestial_body->getMass())/(distance*distance)) * direction;;
        }
        auto shape_matrix = this->getShape()->getPosition();
        
        if(this->isDebugMode()){
          auto shape_position = glm::vec3(shape_matrix[3]);
          std::cout << this->getName() <<  " Shape Position => ("
          << shape_position.x << ", "
          << shape_position.y << ", "
          << shape_position.z << ")" << std::endl;
          std::cout << this->getName() <<  " Physics Acceleration => ("
          << this->velocity.x << ", "
          << this->velocity.y << ", "
          << this->velocity.z << ")" << std::endl;

          std::cout << this->getName() <<  " Physics Velocity => ("
          << this->velocity.x << ", "
          << this->velocity.y << ", "
          << this->velocity.z << ")" << std::endl;

          std::cout << this->getName() <<  " Physics Position => ("
          << this->position.x << ", "
          << this->position.y << ", "
          << this->position.z << ")" << std::endl << std::endl;;
        }        

        this->velocity += this->acceleration * dt;
        this->position += this->velocity * dt;

        const glm::vec3 render_position = this->position * Engines::Physics::Constants::UNIT_SCALE;
        if(this->isDebugMode()){
          std::cout << this->getName() <<  " New Shape Position => ("
          << render_position.x << ", "
          << render_position.y << ", "
          << render_position.z << ")" << std::endl << std::endl;;
        }
        shape_matrix[3] = glm::vec4(render_position, 1.0f);
        this->getShape()->updatePosition(shape_matrix);
      }

      static glm::vec3 calculateOrbitalVelocity(std::shared_ptr<CelestialBody> star, std::shared_ptr<CelestialBody>  planet){
        const glm::vec3 r = planet->getPosition() - star->getPosition();
        glm::vec3 up = glm::vec3(0,1,0);
        if(glm::abs(glm::dot(glm::normalize(r), up)) > 0.99f)
            up = glm::vec3(1,0,0); // choose different axis if parallel

        const glm::vec3 tangent = glm::cross(r, up);
        const glm::vec3 direction = glm::normalize(tangent);
        const float distance = glm::length(r);
        const float orbital_speed = std::sqrt((Engines::Physics::Constants::GRAVITATIONAL_CONSTANT * star->getMass())/distance);

        return direction * orbital_speed;
      }

    private:
      std::string name;
      float radius;
      bool has_orbit = false;
      bool is_star = false;
      bool is_debug_mode = false;
      std::shared_ptr<Engines::Graphics::Shader> orbit_shader;
      glm::vec3 orbit_center{0.0f, 0.0f, 0.0f};
      glm::vec3 orbit_color{0.0f, 0.0f, 0.0f};

      std::shared_ptr<Engines::Graphics::Shape> orbit;
  };
}