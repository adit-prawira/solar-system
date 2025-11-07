#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <engines/graphics/geometry_builder.hpp>
#include <engines/graphics/shader.hpp>
#include <engines/graphics/camera.hpp>

namespace Application {
  static bool is_dragging = false;
  static bool is_rotating = false;
  static double last_x = 0.0;
  static double last_y = 0.0;
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
      static void mouseCallback(GLFWwindow *window, double x_position, double y_position);
      static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
      static void scrollCallback(GLFWwindow *window, double x_offset, double y_offset);

      static void handleLeftClick(GLFWwindow *window, int action);
      static void handleRightClick(GLFWwindow *window, int action);
  };
}