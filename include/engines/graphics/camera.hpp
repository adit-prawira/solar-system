#pragma once
#include <glm/glm.hpp>
#include <engines/graphics/shader.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Engines::Graphics {
  class Camera {
    public:
      Camera& setSpeed(float camera_speed);
      Camera& setShader(std::shared_ptr<Engines::Graphics::Shader> shader);
      Camera& setCameraFront(glm::vec3 camera_front);
      Camera& setCameraAngleLimit(float camera_angle_limit);
      Camera& setCameraPitchLimit(float camera_pitch_limit);
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

      void processMouseScroll(float y_offset);
      void processMouseRotate(float x_offset, float y_offset, bool constraint_pitch);
      void processMouseMove(float x_offset, float y_offset);

    private:
      float camera_speed = 1.0f;
      float mouse_sensitivity = 0.1f;
      float camera_pitch = 0.0f;

      float camera_angle_limit = 89.0f;
      float camera_pitch_limit = -89.0f;

      float camera_angle = 0.0f; // current horizontal rotation angle
      float initial_camera_angle = 0.0f; // current horizontal rotation angle

      std::shared_ptr<Engines::Graphics::Shader> shader = nullptr;
      glm::vec3 camera_front{0.0f, 0.0f, -1.0f};
      glm::vec3 initial_camera_front{0.0f, 0.0f, -1.0f};

      glm::vec3 camera_position{15.0f, 0.0f, 50.0f};
      glm::vec3 initial_camera_position{15.0f, 0.0f, 50.0f};

      template<typename Func>
      void execute(Func &&f);

      void processDirection();
  };
}