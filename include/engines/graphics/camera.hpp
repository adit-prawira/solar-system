#pragma once
#include <glm/glm.hpp>
#include <engines/graphics/shader.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engines::Graphics {
  class Camera {
    public:
      Camera& setSpeed(float camera_speed);
      Camera& setShader(std::shared_ptr<Engines::Graphics::Shader> shader);

      Camera& build();

      void stream();
      glm::mat4 getView();

      void up();
      void down();
      void left();
      void right();
      void zoomIn();
      void zoomOut();
      void rotateClockwise();
      void rotateAntiClockwise();
      void reset();

    private:
      float camera_speed = 1.0f;

      std::shared_ptr<Engines::Graphics::Shader> shader = nullptr;
      glm::vec3 camera_front{0.0f, 0.0f, -1.0f};
      glm::vec3 initial_camera_front{0.0f, 0.0f, -1.0f};

      glm::vec3 camera_position{15.0f, 0.0f, 50.0f};
      glm::vec3 initial_camera_position{15.0f, 0.0f, 50.0f};

      float camera_angle = 0.0f; // current horizontal rotation angle
      float initial_camera_angle = 0.0f; // current horizontal rotation angle

      template<typename Func>
      void execute(Func &&f);

      void processDirection();


  };
}