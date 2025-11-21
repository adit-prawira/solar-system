#pragma once
#include <engines/physics/physics_body.hpp>
#include <engines/physics/constants.hpp>
#include <engines/graphics/geometry_builder.hpp>
#include <engines/maths/converter.hpp>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace Simulation {
  class CelestialBody : public Engines::Physics::PhysicsBody<CelestialBody> {
    public:
      ~CelestialBody(){
        if(this->trail_VBO != 0){
          glDeleteBuffers(1, &this->trail_VBO);
          this->trail_VBO = 0;
        }
        
        if(this->trail_VAO != 0){
          glDeleteBuffers(1, &this->trail_VAO);
          this->trail_VAO = 0;
        }
      }

      CelestialBody& setName(std::string name){
        this->name = name;
        return *this;
      }

      CelestialBody& setIsStar(bool is_star){
        this->is_star = is_star;
        return *this;
      }

      CelestialBody& setIsShowTrail(bool is_show_trail){
        this->is_show_trail = is_show_trail;
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

      CelestialBody& setRenderRadiusMagnification(float magnification){
        this->render_radius_magnification = magnification;
        return *this;
      }

      CelestialBody& setRenderPositionMagnification(float magnification){
        this->render_position_magnification = magnification;
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

      CelestialBody& setTrailShader(std::shared_ptr<Engines::Graphics::Shader> trail_shader){
        this->trail_shader = trail_shader;
        return *this;
      }

      glm::vec3 getRenderPosition()  {
        // Base conversion using your existing meterToPixel function
        float x_pixel = Engines::Maths::Converter::meterToPixel(this->position.x, &this->render_position_magnification);
        float y_pixel = Engines::Maths::Converter::meterToPixel(this->position.y, &this->render_position_magnification);
        float z_pixel = Engines::Maths::Converter::meterToPixel(this->position.z, &this->render_position_magnification);

        // Apply a visual zoom factor to exaggerate movement
        const float visual_zoom = 100.0f; // Adjust as needed for screen size
        // x_pixel *= visual_zoom;
        z_pixel *= visual_zoom;

        // Optional: exaggerate vertical separation for clarity
        const float vertical_scale = 1.0f; // Keep 1.0f if you don't want to exaggerate Y
        y_pixel *= vertical_scale;

        return glm::vec3(x_pixel, y_pixel, z_pixel);
      }

      float getRenderRadius(){
        return Engines::Maths::Converter::meterToPixel(this->radius, &this->render_radius_magnification);
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
          auto ring_model_matrix = glm::translate(glm::mat4(1.0f), this->orbit_center);
          this->orbit = Engines::Graphics::GeometryBuilder::createRing()
          .setOuterRadius(this->getRenderPosition().x)
          .setInnerRadius(this->getRenderPosition().x - 3.0f)
          .setCenter(glm::vec3(0.0f, 0.0f, 0.0f))
          .setModelMatrix(ring_model_matrix)
          .setColor(this->orbit_color)
          .setShader(this->orbit_shader)
          .build();
        }
        return *this; 
      };
      
      float getRadius(){return radius;};

      void revolve(std::vector<std::shared_ptr<CelestialBody>> celestial_bodies, const float dt){
        auto old_acceleration = this->acceleration;
        this->acceleration = glm::vec3(0.0f);
        const float min_distance = 0.001f;

        for(auto &celestial_body : celestial_bodies){
          if(celestial_body.get() == this) continue;
          
          const glm::vec3 r = celestial_body->getPosition() - this->getPosition();
          float distance = glm::length(r);
          
          // clamp distance
          distance = glm::max(distance, min_distance);

          // unit vector determines direction r^
          const glm::vec3 direction = glm::normalize(r);
          const float magnitude_acceleration = (Engines::Physics::Constants::GRAVITATIONAL_CONSTANT  * celestial_body->getMass())/(distance*distance);
          this->acceleration += direction * magnitude_acceleration * 5000.f;
        }
        auto shape_matrix = this->getShape()->getModelMatrix();
        
        if(this->isDebugMode()){
          auto shape_position = glm::vec3(shape_matrix[3]);
          std::cout << this->getName() <<  " Shape Position => ("
          << shape_position.x << ", "
          << shape_position.y << ", "
          << shape_position.z << ")" << std::endl;
          std::cout << this->getName() <<  " Physics Acceleration => ("
          << this->acceleration.x << ", "
          << this->acceleration.y << ", "
          << this->acceleration.z << ")" << std::endl;

          std::cout << this->getName() <<  " Physics Velocity => ("
          << this->velocity.x << ", "
          << this->velocity.y << ", "
          << this->velocity.z << ")" << std::endl;

          std::cout << this->getName() <<  " Physics Position => ("
          << this->position.x << ", "
          << this->position.y << ", "
          << this->position.z << ")" << std::endl << std::endl;;
        }        

        this->position += this->velocity*dt + 0.5f*old_acceleration*dt*dt;
        this->velocity += 0.5f * (old_acceleration + this->acceleration)*dt;

        glm::vec3 render_position = this->getRenderPosition();
        this->trails.push_back(render_position);

        if(this->trails.size() > this->trail_size)
          this->trails.erase(this->trails.begin());
        
        if(this->isDebugMode()){
          std::cout << this->getName() <<  " New Shape Position => ("
          << render_position.x << ", "
          << render_position.y << ", "
          << render_position.z << ")" << std::endl << std::endl;
        }
        shape_matrix[3] = glm::vec4(render_position, 1.0f);

        this->getShape()->updateModelMatrix(shape_matrix);
      }

      void setupTrail(){
        glGenVertexArrays(1, &this->trail_VAO);
        glGenBuffers(1, &this->trail_VBO);

        glBindVertexArray(this->trail_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->trail_VBO);

        // reserve enough size
        glBufferData(GL_ARRAY_BUFFER, this->trail_size * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glEnableVertexAttribArray(0);
      }

      void drawTrail(){
        if(this->trails.empty()) return;

        this->trail_shader->setMat4("model", glm::mat4(1.0f));
        this->trail_shader->setVec3("line_color", this->getShape()->getColor());

        glBindVertexArray(this->trail_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, this->trail_VBO);

        glBufferSubData(GL_ARRAY_BUFFER, 0,
          this->trails.size() * sizeof(glm::vec3),
          this->trails.data());

        glLineWidth(2.0f);
        glDrawArrays(GL_LINE_STRIP, 0, this->trails.size());
      }

      static glm::vec3 calculateOrbitalVelocity(std::string planet_name, std::shared_ptr<CelestialBody> star, std::shared_ptr<CelestialBody>  planet, bool is_debug = false){
        // convert pixels to meter by dividing with unit scales
        glm::vec3 r = planet->getPosition() - star->getPosition();
        const float distance = glm::length(r);
        const float orbital_speed = std::sqrt((Engines::Physics::Constants::GRAVITATIONAL_CONSTANT * star->getMass())/distance);
        auto orbital_velocity = glm::vec3(0.0f, 0.0f, orbital_speed);
        if(is_debug){
          std::cout << "========== " << planet_name << " ==========" << std::endl; 
          std::cout <<"Diff Position => ("
            << r.x << ", "
            << r.y << ", "
            << r.z << ")" << std::endl;
          std::cout <<"Orbital Velocity => ("
            << orbital_velocity.x << ", "
            << orbital_velocity.y << ", "
            << orbital_velocity.z << ")" << std::endl << std::endl;
        }
        return orbital_velocity;
      }

    private:
      std::string name;
      float radius;
      float render_radius_magnification = 1.0f;
      float render_position_magnification = 1.0f;

      unsigned int trail_size = 500;
      std::vector<glm::vec3> trails;
      bool is_show_trail = false;
      bool has_orbit = false;
      bool is_star = false;
      bool is_debug_mode = false;
      std::shared_ptr<Engines::Graphics::Shader> orbit_shader;
      
      std::shared_ptr<Engines::Graphics::Shader> trail_shader;

      glm::vec3 orbit_center{0.0f, 0.0f, 0.0f};
      glm::vec3 orbit_color{0.0f, 0.0f, 0.0f};

      std::shared_ptr<Engines::Graphics::Shape> orbit;

      GLuint trail_VAO, trail_VBO;
  };
}