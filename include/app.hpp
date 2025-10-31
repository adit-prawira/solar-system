#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include <engines/graphics/geometry_builder.hpp>
#include <engines/graphics/shader.hpp>

namespace Application {
  class App {
    const unsigned int SCR_WIDTH = 1080;
    const unsigned int SCR_HEIGHT = 840;

    public:
      App(const std::string name);

      void run();

      float aspectRatio(){
        return float(SCR_WIDTH) / float(SCR_HEIGHT);
      }
    private:
      std::string name;
      float camera_speed = 1.0f;
      float yaw = -90.0f;
      float pitch = 0.0f;
      
      glm::vec3 camera_front{0.0f, 0.0f, -1.0f};
      glm::vec3 initial_camera_front{0.0f, 0.0f, -1.0f};

      glm::vec3 camera_position{15.0f, 0.0f, 50.0f};
      glm::vec3 initial_camera_position{15.0f, 0.0f, 50.0f};


      float camera_angle = 0.0f; // current horizontal rotation angle
      float initial_camera_angle = 0.0f; // current horizontal rotation angle

      glm::vec3 light_position{2.0f, 4.0f, 2.0f};

      void processInput(GLFWwindow *window);
      static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
  };
}