#include <engines/graphics/camera.hpp>

namespace Engines::Graphics{
  Camera& Camera::setSpeed(float speed){
    this->camera_speed = speed;
    return *this;
  }

  Camera& Camera::setShader(std::shared_ptr<Engines::Graphics::Shader> shader){
    this->shader = shader;
    return *this;
  }

  Camera& Camera::setCameraFront(glm::vec3 camera_front){
    this->camera_front = camera_front;
    return *this;
  }

  Camera& Camera::setCameraAngleLimit(float camera_angle_limit){
    this->camera_angle_limit = camera_angle_limit;
    return *this;
  }

  Camera& Camera::setCameraPitchLimit(float camera_pitch_limit){
    this->camera_pitch_limit = camera_pitch_limit;
    return *this;
  }

  Camera& Camera::build(){
    if(this->shader == nullptr) throw std::runtime_error("Camera_ERROR => Shader is required for Camera");
    return *this;
  }

  glm::mat4 Camera::getView(){
    return glm::lookAt(this->camera_position, this->camera_position + this->camera_front, glm::vec3(0, 1, 0));
  }

  void Camera::stream(){
    shader->setMat4("view", this->getView());
    shader->setVec3("view_position", this->camera_position);
  }

  void Camera::left(){
    this->execute([this](){
      this->camera_position.x -= this->camera_speed;
    });
  }

  void Camera::right(){
    this->execute([this](){
      this->camera_position.x += this->camera_speed;
    });
  }

  void Camera::up(){
    this->execute([this](){
      this->camera_position.y += this->camera_speed;
    });
  }

  void Camera::down(){
    this->execute([this](){
      this->camera_position.y -= this->camera_speed;
    });
  }
  
  void Camera::zoomIn(){
    this->execute([this](){
      this->camera_position.z -= this->camera_speed;
    });
  }

  void Camera::zoomOut(){
    this->execute([this](){
      this->camera_position.z += this->camera_speed;
    });
  }

  void Camera::rotateClockwise(){
    this->execute([this](){
      this->camera_angle -= 1.0f;
    });
  }

  void Camera::rotateAntiClockwise(){
    this->execute([this](){
      this->camera_angle += 1.0f;
    });
  }

  void Camera::reset(){
    this->execute([this](){
      this->camera_position = this->initial_camera_position;
      this->camera_front = this->initial_camera_front;
      this->camera_angle = this->initial_camera_angle;
    });
  }

  void Camera::processMouseRotate(float x_offset, float y_offset, bool constraint_pitch){
    this->execute([this, &x_offset, &y_offset, constraint_pitch](){
      x_offset *= this->mouse_sensitivity;
      y_offset *= this->mouse_sensitivity;

      this->camera_angle += x_offset;
      this->camera_pitch += y_offset;

      if(constraint_pitch){
        if(this->camera_angle > this->camera_angle_limit) this->camera_angle = this->camera_angle_limit;
        if(this->camera_pitch < this-> camera_pitch_limit) this->camera_pitch = this->camera_pitch_limit;
      }
    });
  }
  
  void Camera::processMouseMove(float x_offset, float y_offset){
    this->execute([this, &x_offset, &y_offset](){
      x_offset *= this->mouse_sensitivity;
      y_offset *= this->mouse_sensitivity;

      this->camera_position.x += -x_offset;
      this->camera_position.y += y_offset;
    }); 
  }

  void Camera::processMouseScroll(float y_offset){
    this->camera_position += this->camera_front * (y_offset * this->camera_speed);
  }
  
  template<typename Func>
  void Camera::execute(Func &&f){
    f();
    this->processDirection();
  }

  void Camera::processDirection(){
    // y-axis rotation angle
    float yaw = glm::radians(this->camera_angle);

    // x-axis rotation angle
    float pitch = glm::radians(this->camera_pitch);

    this->camera_front = glm::normalize(glm::vec3(
        cos(pitch)*sin(yaw), 
        sin(pitch), 
        -cos(pitch) * cos(yaw)
    ));
  }
}