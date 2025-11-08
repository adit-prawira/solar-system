#include <app.hpp>

namespace Application {
  App::App(const std::string name):name(name){}

  void App::run(){

    std::string input_flamm_paraboloid;
    std::cout << "Render Flamm Paraboloid (y/n) => ";
    std::cin >> input_flamm_paraboloid;
    bool show_flamm_paraboloid = input_flamm_paraboloid=="y";

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow *window = glfwCreateWindow(Config::SCR_WIDTH, Config::SCR_HEIGHT, this->name.c_str(), NULL, NULL);
    if (window == NULL)
    {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return;
    }
    glEnable(GL_DEPTH_TEST);

    // Shader
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), this->aspectRatio(), 0.5f, 10000.0f);
    auto shader = std::make_shared<Engines::Graphics::Shader>("shaders/vertex.glsl", "shaders/fragment.glsl");
    this->camera = std::make_unique<Engines::Graphics::Camera>();
    this->camera->setShader(shader)
      .setSpeed(15.0f)
      .build();

    glfwSetWindowUserPointer(window, this->camera.get());
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback); // if you use scroll to zoom

    const float distance_magnification = 100.0f, surface_size = 200;    
    const float distance_sun_to_mercury = Engines::Maths::Converter::scaleDistanceBySurfaceSize(Engines::Physics::Constants::DISTANCE_SUN_MERCURY_M,surface_size,distance_magnification),
      distance_sun_to_venus = Engines::Maths::Converter::scaleDistanceBySurfaceSize(Engines::Physics::Constants::DISTANCE_SUN_VENUS_M,surface_size,distance_magnification),
      distance_sun_to_earth = Engines::Maths::Converter::scaleDistanceBySurfaceSize(Engines::Physics::Constants::DISTANCE_SUN_EARTH_M,surface_size,distance_magnification),
      distance_sun_to_mars = Engines::Maths::Converter::scaleDistanceBySurfaceSize(Engines::Physics::Constants::DISTANCE_SUN_MARS_M,surface_size,distance_magnification),
      distance_sun_to_jupiter = Engines::Maths::Converter::scaleDistanceBySurfaceSize(Engines::Physics::Constants::DISTANCE_SUN_JUPITER_M,surface_size,distance_magnification/2),
      distance_sun_to_saturn = Engines::Maths::Converter::scaleDistanceBySurfaceSize(Engines::Physics::Constants::DISTANCE_SUN_SATURN_M,surface_size,distance_magnification/3),
      distance_sun_to_uranus = Engines::Maths::Converter::scaleDistanceBySurfaceSize(Engines::Physics::Constants::DISTANCE_SUN_URANUS_M,surface_size,distance_magnification/5),
      distance_sun_to_neptune = Engines::Maths::Converter::scaleDistanceBySurfaceSize(Engines::Physics::Constants::DISTANCE_SUN_NEPTUNE_M,surface_size,distance_magnification/7),
      distance_sun_to_pluto = Engines::Maths::Converter::scaleDistanceBySurfaceSize(Engines::Physics::Constants::DISTANCE_SUN_PLUTO_M,surface_size,distance_magnification/8);

    std::vector<std::shared_ptr<Simulation::CelestialBody>> celestial_bodies;
    auto sun = Simulation::Sun()
      .setColor(glm::vec3(1.0f, 0.3f, 0.0f))
      .setPosition(glm::vec3(0.0f, 50.0f, 0.0f))
      .setMagnification(2.0f)
      .build(shader),
      mercury = Simulation::Mercury()
      .setPosition(glm::vec3(distance_sun_to_mercury, 50.0f, 0.0f))
      .setColor(glm::vec3(0.6f, 0.5f, 0.4f))
      .setMagnification(10.0f)
      .build(shader),
      venus = Simulation::Venus()
          .setPosition(glm::vec3(distance_sun_to_venus, 50.0f, 0.0f))
          .setColor(glm::vec3(0.9f, 0.8f, 0.5f))
          .setMagnification(10.0f)
          .build(shader),
      earth = Simulation::Earth()
          .setPosition(glm::vec3(distance_sun_to_earth, 50.0f, 0.0f))
          .setColor(glm::vec3(0.2f, 0.4f, 1.0f))
          .setMagnification(10.0f)
          .build(shader),
      mars = Simulation::Earth()
          .setPosition(glm::vec3(distance_sun_to_mars, 50.0f, 0.0f))
          .setColor(glm::vec3(1.0f, 0.3f, 0.0f))
          .setMagnification(10.0f)
          .build(shader),
      jupiter = Simulation::Jupiter()
          .setPosition(glm::vec3(distance_sun_to_jupiter, 50.0f, 0.0f))
          .setColor(glm::vec3(0.9f, 0.7f, 0.5f))
          .setMagnification(5.0f)
          .build(shader),
      saturn = Simulation::Saturn()
          .setPosition(glm::vec3(distance_sun_to_saturn, 50.0f, 0.0f))
          .setColor(glm::vec3(0.9f, 0.8f, 0.5f))
          .setMagnification(5.0f)
          .build(shader),
      uranus = Simulation::Uranus()
          .setPosition(glm::vec3(distance_sun_to_uranus, 50.0f, 0.0f))
          .setColor(glm::vec3(0.2f, 0.4f, 1.0f))
          .setMagnification(7.0f)
          .build(shader),
      neptune = Simulation::Neptune()
          .setPosition(glm::vec3(distance_sun_to_neptune, 50.0f, 0.0f))
          .setColor(glm::vec3(0.3f, 0.3f, 1.0f))
          .setMagnification(7.0f)
          .build(shader),
      pluto = Simulation::Pluto()
          .setPosition(glm::vec3(distance_sun_to_pluto, 50.0f, 0.0f))
          .setColor(glm::vec3(0.8f, 0.7f, 0.6f))
          .setMagnification(20.0f)
          .build(shader);

    celestial_bodies.insert(celestial_bodies.end(), {
      sun, mercury, venus,
      earth, mars, jupiter,
      saturn, uranus, neptune,
      pluto
    });

    glm::mat4 surface_position = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    auto surface_shape = Engines::Graphics::GeometryBuilder::createSurfaceGrid()
      .setRows(static_cast<int>(surface_size))
      .setColumns(static_cast<int>(surface_size))
      .setSpace(20.0f)
      .setPosition(surface_position)
      .setColor(glm::vec3(0.2f, 0.2f, 0.2f))
      .setShader(shader)
      .build();
    Engines::Graphics::Surface* raw = dynamic_cast<Engines::Graphics::Surface*>(surface_shape.get());
    if (!raw) throw std::runtime_error("Builder did not return a Surface");
    std::unique_ptr<Engines::Graphics::Surface> raw_surface_shape(static_cast<Engines::Graphics::Surface*>(surface_shape.release()));
    Simulation::FlammParaboloid surface;
    surface.setSurface(std::move(raw_surface_shape))
      .build();

    while (!glfwWindowShouldClose(window))
    {
      this->processInput(window);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      shader->use();
      shader->setMat4("projection", projection);
      shader->setVec3("light_position", this->light_position);
      shader->setVec3("light_color", glm::vec3(1.0f));
      this->camera->stream();

      if(show_flamm_paraboloid){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        surface.apply(celestial_bodies);
        surface.getSurface()->draw();
      }
  
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      for(auto &celestial_body : celestial_bodies)
        celestial_body->getShape()->draw();

      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    glfwTerminate();
  }

  void App::processInput(GLFWwindow *window){
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      this->camera->left();
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      this->camera->right();
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      this->camera->up();
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      this->camera->down();

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
      this->camera->zoomIn();
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
      this->camera->zoomOut();

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        this->camera->rotateClockwise();
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        this->camera->rotateAntiClockwise();
    
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
      this->camera->reset();
  }

  void App::framebufferSizeCallback([[maybe_unused]]GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
  }

  void App::mouseCallback(GLFWwindow *window, double x_position, double y_position){
    auto* camera = static_cast<Engines::Graphics::Camera*>(glfwGetWindowUserPointer(window));
    if(!camera) return;
    
    if(!Application::is_dragging && !Application::is_rotating){
      Application::last_x = x_position;
      Application::last_y = y_position;
      return;
    }
    float x_offset = static_cast<float>(x_position - Application::last_x);
    float y_offset = static_cast<float>(Application::last_y - y_position);
    Application::last_x = x_position;
    Application::last_y = y_position;

    if(Application::is_rotating) camera->processMouseRotate(x_offset, y_offset, true);
    if(Application::is_dragging) camera->processMouseMove(x_offset, y_offset);
  }
  
  void App::mouseButtonCallback(GLFWwindow *window, int button, int action, [[maybe_unused]]int mods){
    if(button == GLFW_MOUSE_BUTTON_RIGHT) handleRightClick(window, action);
    if(button == GLFW_MOUSE_BUTTON_LEFT) handleLeftClick(window, action);
  }

  void App::handleLeftClick(GLFWwindow *window, int action){
    if(action != GLFW_PRESS){
      Application::is_dragging = false;
      return;
    }
    Application::is_dragging = true;
    glfwGetCursorPos(window, &Application::last_x, &Application::last_y);
  }

  void App::handleRightClick(GLFWwindow *window, int action){
    if(action != GLFW_PRESS){
      Application::is_rotating = false;
      return;
    }
    Application::is_rotating = true;
    glfwGetCursorPos(window, &Application::last_x, &Application::last_y);
  }

  void App::scrollCallback(GLFWwindow *window, [[maybe_unused]]double x_offset, double y_offset){
    auto* camera = static_cast<Engines::Graphics::Camera*>(glfwGetWindowUserPointer(window));
    if(camera) camera->processMouseScroll(static_cast<float>(y_offset));
  }
}