#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

#include <engines/graphics/shape.hpp>
#include <glm/glm.hpp>

namespace Engines::Physics {

  template<typename T>
  class PhysicsBody {
    public:
      T& setMass(double mass){
        this->mass = mass;
        return static_cast<T&>(*this);
      }
      T& setPosition(glm::dvec3 position){
        this->position = position;
        return static_cast<T&>(*this);
      }
      T& setVelocity(glm::dvec3 velocity){
        this->velocity = velocity;
        return static_cast<T&>(*this);
      }
      T& setAcceleration(glm::dvec3 acceleration){
        this->acceleration = acceleration;
        return static_cast<T&>(*this);
      }

      T& setShape(std::unique_ptr<Engines::Graphics::Shape> shape){
        this->shape = std::move(shape); // actually store the shape
        return static_cast<T&>(*this);
      }

      virtual T& build() = 0;

      double getMass() const {return mass;};
      Engines::Graphics::Shape* getShape() const {return this->shape.get();}
      glm::dvec3 getAcceleration() const {return acceleration;};
      glm::dvec3 getVelocity() const {return velocity;};
      glm::dvec3 getPosition() const {return position;};
      
    protected:
      double mass;
      std::unique_ptr<Engines::Graphics::Shape> shape;
      glm::dvec3 position{0.0, 0.0, 0.0};
      glm::dvec3 velocity{0.0, 0.0, 0.0};
      glm::dvec3 acceleration{0.0, 0.0, 0.0};
      std::vector<std::string> validation_messages;

      template<typename Func>
      void executeValidation(Func &&f){
        f();
        if(!this->mass) this->validation_messages.push_back("VALIDATION => Mass is required for physics body");
        if(!this->validation_messages.empty()) throw std::runtime_error(this->compileValidationMessages());
      }
    private:
      std::string compileValidationMessages(){
        std::ostringstream oss;
        for(size_t i = 0; i < this->validation_messages.size(); ++i){
          oss << this->validation_messages[i];
          if(i != this->validation_messages.size() - 1) oss << "\n";
        }
        return oss.str();
      };
      friend T;
  };
}
