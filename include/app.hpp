#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <engines/graphics/geometry_builder.hpp>
#include <engines/graphics/shader.hpp>
#include <engines/graphics/camera.hpp>

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
      std::unique_ptr<Engines::Graphics::Camera> camera = nullptr;
      glm::vec3 light_position{2.0f, 4.0f, 2.0f};

      void processInput(GLFWwindow *window);

      static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
  };
}