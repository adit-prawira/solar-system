#include <app.hpp>

namespace Application {
  App::App(const std::string name):name(name){}

  void App::run(){
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
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), this->aspectRatio(), 0.5f, 30000.0f);
    auto shader = std::make_shared<Engines::Graphics::Shader>("shaders/shape.vert", "shaders/shape.frag");
    auto trail_shader = std::make_shared<Engines::Graphics::Shader>("shaders/trail.vert", "shaders/trail.frag");

    this->camera = std::make_unique<Engines::Graphics::Camera>();
    this->camera->setShader(shader)
      .setTrailShader(trail_shader)
      .setSpeed(20.0f)
      .build();

    glfwSetWindowUserPointer(window, this->camera.get());
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback); // if you use scroll to zoom

    auto celestial_bodies = this->generateCelestialBodies(shader);
    auto sun = celestial_bodies["Sun"];
    
    std::vector<std::shared_ptr<Simulation::CelestialBody>> stars;
    stars.push_back(sun);
    celestial_bodies.erase("Sun");
    
    for(auto &[name, celestial_body] : celestial_bodies)
      celestial_body->setOrbitalVelocity(Simulation::CelestialBody::calculateOrbitalVelocity(name, sun, celestial_body, true));
      
    auto flamm_paraboloid_surface = this->generateFlammParaboloid(shader);

    while (!glfwWindowShouldClose(window))
    {
      this->processInput(window);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      shader->use();
      shader->setMat4("projection", projection);
      shader->setVec3("light_position", this->light_position);
      shader->setVec3("light_color", glm::vec3(1.0f));

      trail_shader->use();
      trail_shader->setMat4("projection", projection);

      this->camera->stream();
            
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      
      for(auto &star : stars)
        star->getShape()->draw();

      for(auto &[_, celestial_body] : celestial_bodies){
        celestial_body->revolve(stars, 1.0);
        celestial_body->getShape()->draw();
        celestial_body->getOrbit()->draw();
      }

      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      flamm_paraboloid_surface.apply(stars);
      flamm_paraboloid_surface.getSurface()->draw();
      
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
      this->camera->forward();
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      this->camera->backward();

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
      this->camera->up();
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
      this->camera->down();

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

  std::unordered_map<std::string, std::shared_ptr<Simulation::CelestialBody>> App::generateCelestialBodies(std::shared_ptr<Engines::Graphics::Shader> shader){
    const float sun_radius_magnification = 1500, 
      planet_radius_magnification = 5100,
      distance_magnification = 20;
    const auto orbit_center = glm::dvec3(0.0, 50.0, 0.0);
    auto sun = Simulation::Sun()
        .setIsStar(true)
        .setColor(glm::vec3(1.0f, 0.3f, 0.0f))
        .setPosition(glm::dvec3(0.0, 50.0, 0.0))
        .setRenderRadiusMagnification(sun_radius_magnification)
        .build(shader);

    std::vector<std::shared_ptr<Simulation::CelestialBody>> celestial_bodies{
      Simulation::Mercury()
        .setPosition(sun->getPosition() + glm::dvec3(Engines::Physics::Constants::DISTANCE_SUN_MERCURY_M, 0.0, 0.0))
        .setColor(glm::vec3(0.6f, 0.5f, 0.4f))
        .setRenderRadiusMagnification(planet_radius_magnification)
        .setRenderPositionMagnification(distance_magnification)
        .setHasOrbit(true)
        .setOrbitCenter(orbit_center)
        .build(shader), 
      Simulation::Venus()
        .setPosition(sun->getPosition() + glm::dvec3(Engines::Physics::Constants::DISTANCE_SUN_VENUS_M, 0.0, 0.0))
        .setColor(glm::vec3(0.9f, 0.8f, 0.5f))
        .setRenderRadiusMagnification(planet_radius_magnification)
        .setRenderPositionMagnification(distance_magnification)
        .setHasOrbit(true)
        .setOrbitCenter(orbit_center)
        .build(shader),
      Simulation::Earth()
        .setPosition(sun->getPosition() + glm::dvec3(Engines::Physics::Constants::DISTANCE_SUN_EARTH_M, 0.0, 0.0))
        .setColor(glm::vec3(0.2f, 0.4f, 1.0f))
        .setRenderRadiusMagnification(planet_radius_magnification)
        .setRenderPositionMagnification(distance_magnification)
        .setHasOrbit(true)
        .setOrbitCenter(orbit_center)
        .build(shader), 
      Simulation::Mars()
        .setPosition(sun->getPosition() + glm::dvec3(Engines::Physics::Constants::DISTANCE_SUN_MARS_M, 0.0, 0.0))
        .setColor(glm::vec3(1.0f, 0.3f, 0.0f))
        .setRenderRadiusMagnification(planet_radius_magnification)
        .setRenderPositionMagnification(distance_magnification)
        .setHasOrbit(true)
        .setOrbitCenter(orbit_center)
        .build(shader), 
      Simulation::Jupiter()
        .setPosition(sun->getPosition() + glm::dvec3(Engines::Physics::Constants::DISTANCE_SUN_JUPITER_M, 0.0, 0.0))
        .setColor(glm::vec3(0.9f, 0.7f, 0.5f))
        .setRenderRadiusMagnification(planet_radius_magnification)
        .setRenderPositionMagnification(distance_magnification/2)
        .setHasOrbit(true)
        .setOrbitCenter(orbit_center)
        .build(shader),
      Simulation::Saturn()
        .setPosition(sun->getPosition() + glm::dvec3(Engines::Physics::Constants::DISTANCE_SUN_SATURN_M, 0.0, 0.0))
        .setColor(glm::vec3(0.9f, 0.8f, 0.5f))
        .setRenderRadiusMagnification(planet_radius_magnification)
        .setRenderPositionMagnification(distance_magnification/2)
        .setHasOrbit(true)
        .setOrbitCenter(orbit_center)
        .build(shader), 
      Simulation::Uranus()
        .setPosition(sun->getPosition() + glm::dvec3(Engines::Physics::Constants::DISTANCE_SUN_URANUS_M, 0.0, 0.0))
        .setColor(glm::vec3(0.2f, 0.4f, 1.0f))
        .setRenderRadiusMagnification(planet_radius_magnification)
        .setRenderPositionMagnification(distance_magnification/3)
        .setHasOrbit(true)
        .setOrbitCenter(orbit_center)
        .build(shader), 
      Simulation::Neptune()
        .setPosition(sun->getPosition() + glm::dvec3(Engines::Physics::Constants::DISTANCE_SUN_NEPTUNE_M, 0.0, 0.0))
        .setColor(glm::vec3(0.3f, 0.3f, 1.0f))
        .setRenderRadiusMagnification(planet_radius_magnification)
        .setRenderPositionMagnification(distance_magnification/4)
        .setHasOrbit(true)
        .setOrbitCenter(orbit_center)
        .build(shader),
      Simulation::Pluto()
        .setPosition(sun->getPosition() + glm::dvec3(Engines::Physics::Constants::DISTANCE_SUN_PLUTO_M, 0.0, 0.0))
        .setColor(glm::vec3(0.8f, 0.7f, 0.6f))
        .setRenderRadiusMagnification(planet_radius_magnification)
        .setRenderPositionMagnification(distance_magnification/4)
        .setHasOrbit(true)
        .setOrbitCenter(orbit_center)
        .build(shader)
    };
    celestial_bodies.push_back(sun);
    std::unordered_map<std::string, std::shared_ptr<Simulation::CelestialBody>> celestial_map;
    for(auto &celestial_body : celestial_bodies)
      celestial_map[celestial_body->getName()] = celestial_body;
    return celestial_map;
  }

  Simulation::FlammParaboloid App::generateFlammParaboloid(std::shared_ptr<Engines::Graphics::Shader> shader){
    const float surface_size = 80;    
    glm::mat4 surface_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0, 0.0));
    auto surface_shape = Engines::Graphics::GeometryBuilder::createSurfaceGrid()
      .setRows(static_cast<int>(surface_size))
      .setColumns(static_cast<int>(surface_size))
      .setSpace(200.0f)
      .setModelMatrix(surface_matrix)
      .setColor(glm::vec3(0.2f, 0.2f, 0.2f))
      .setShader(shader)
      .build();
    Engines::Graphics::Surface* raw = dynamic_cast<Engines::Graphics::Surface*>(surface_shape.get());
    if (!raw) throw std::runtime_error("Builder did not return a Surface");
    std::unique_ptr<Engines::Graphics::Surface> raw_surface_shape(static_cast<Engines::Graphics::Surface*>(surface_shape.release()));
    Simulation::FlammParaboloid surface;
    surface.setSurface(std::move(raw_surface_shape))
      .build();
    return surface;
  }
}
