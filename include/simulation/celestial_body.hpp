#pragma once
#include <engines/physics/physics_body.hpp>

namespace Simulation {
  class CelestialBody : public Engines::Physics::PhysicsBody<CelestialBody> {
    public:
      CelestialBody& setRadius(float radius){
        this->radius = radius;
        return *this;
      }
      CelestialBody& build() override {
        this->executeValidation([this](){
          if(!this->radius) this->validation_messages.push_back("Radius is required for celestial body");
        });
        return *this; 
      };
      float getRadius(){return radius;};

    private:
      float radius;
  };
}