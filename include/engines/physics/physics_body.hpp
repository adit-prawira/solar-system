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
      T& setMass(float mass){
        this->mass = mass;
        return static_cast<T&>(*this);
      }
      T& setPosition(glm::vec3 position){
        this->position = position;
        return static_cast<T&>(*this);
      }
      T& setVelocity(glm::vec3 velocity){
        this->velocity = velocity;
        return static_cast<T&>(*this);
      }
      T& setAcceleration(glm::vec3 acceleration){
        this->acceleration = acceleration;
        return static_cast<T&>(*this);
      }

      T& setShape(std::unique_ptr<Engines::Graphics::Shape> shape){
        this->shape = std::move(shape); // actually store the shape
        return static_cast<T&>(*this);
      }

      virtual T& build() = 0;

      float getMass() const {return mass;};
      Engines::Graphics::Shape* getShape() const {return this->shape.get();}
      glm::vec3 getAcceleration() const {return acceleration;};
      glm::vec3 getVelocity() const {return velocity;};
      glm::vec3 getPosition() const {return position;};

      virtual void move()  = 0;

    protected:
      float mass;
      std::unique_ptr<Engines::Graphics::Shape> shape;
      glm::vec3 position{0.0f, 0.0f, 0.0f};
      glm::vec3 velocity{0.0f, 0.0f, 0.0f};
      glm::vec3 acceleration{0.0f, 0.0f, 0.0f};
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
