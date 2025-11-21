#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include <engines/maths/converter.hpp>

#include <engines/graphics/geometry_builder.hpp>
#include <engines/graphics/shader.hpp>
#include <engines/graphics/camera.hpp>
#include <simulation/sun.hpp>
#include <simulation/mercury.hpp>
#include <simulation/venus.hpp>
#include <simulation/earth.hpp>
#include <simulation/mars.hpp>
#include <simulation/jupiter.hpp>
#include <simulation/saturn.hpp>
#include <simulation/uranus.hpp>
#include <simulation/neptune.hpp>
#include <simulation/pluto.hpp>

#include <simulation/flamm_paraboloid.hpp>
#include <config/constant.hpp>

namespace Application {
  static bool is_dragging = false;
  static bool is_rotating = false;
  static double last_x = 0.0;
  static double last_y = 0.0;
  class App {
    public:
      App(const std::string name);

      void run();

      float aspectRatio(){
        return float(Config::SCR_WIDTH) / float(Config::SCR_HEIGHT);
      } 

    private:
      std::string name;
      std::unique_ptr<Engines::Graphics::Camera> camera = nullptr;
      glm::vec3 light_position{2.0f, 4.0f, 2.0f};

      void processInput(GLFWwindow *window);
      std::vector<std::shared_ptr<Simulation::CelestialBody>> generateCelestialBodies(std::shared_ptr<Engines::Graphics::Shader> shader);
      Simulation::FlammParaboloid generateFlammParaboloid(std::shared_ptr<Engines::Graphics::Shader> shader);
      static void framebufferSizeCallback(GLFWwindow *window, int width, int height);
      static void mouseCallback(GLFWwindow *window, double x_position, double y_position);
      static void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
      static void scrollCallback(GLFWwindow *window, double x_offset, double y_offset);

      static void handleLeftClick(GLFWwindow *window, int action);
      static void handleRightClick(GLFWwindow *window, int action);
  };
}