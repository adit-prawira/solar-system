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

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, this->name.c_str(), NULL, NULL);
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
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), this->aspectRatio(), 0.5f, 1000.0f);
    auto shader = std::make_shared<Engines::Graphics::Shader>("shaders/vertex.glsl", "shaders/fragment.glsl");
    this->camera = std::make_unique<Engines::Graphics::Camera>();
    this->camera->setShader(shader)
      .setSpeed(1.0f)
      .build();

    glfwSetWindowUserPointer(window, this->camera.get());
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback); // if you use scroll to zoom

    glm::mat4 sun_position = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 20.0f, 0.0f));
    auto sun = Engines::Graphics::GeometryBuilder::createSphere()
            .setRadius(15.0f) 
            .setSectorCount(30)
            .setStackCount(18)
            .setPosition(sun_position)
            .setColor(glm::vec3(1.0f, 1.0f, .3f))
            .setShader(shader)
            .build();

    glm::mat4 mercury_position = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 20.0f, 0.0f));
    auto mercury = Engines::Graphics::GeometryBuilder::createSphere()
            .setRadius(2.0f) 
            .setSectorCount(30)
            .setStackCount(18)
            .setPosition(mercury_position)
            .setColor(glm::vec3(1.0f, 0.5f, 0.3f))
            .setShader(shader)
            .build();
    
    glm::mat4 surface_position = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    auto surface = Engines::Graphics::GeometryBuilder::createSurfaceGrid()
      .setRows(20)
      .setColumns(20)
      .setSpace(10.0f)
      .setPosition(surface_position)
      .setColor(glm::vec3(1.0f, 1.0f, 1.0f))
      .setShader(shader)
      .build();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
      this->processInput(window);

      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      shader->use();
      shader->setMat4("projection", projection);
      shader->setVec3("light_position", this->light_position);
      shader->setVec3("light_color", glm::vec3(1.0f));
      this->camera->stream();

      surface->draw();
      sun->draw();
      mercury->draw();

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
    std::cout << "(x, y) = " << "(" << Application::last_x << ", " << Application::last_y << ")" << std::endl;
  }

  void App::handleRightClick(GLFWwindow *window, int action){
    if(action != GLFW_PRESS){
      Application::is_rotating = false;
      return;
    }
    Application::is_rotating = true;
    glfwGetCursorPos(window, &Application::last_x, &Application::last_y);
    std::cout << "(x, y) = " << "(" << Application::last_x << ", " << Application::last_y << ")" << std::endl;
  }

  void App::scrollCallback(GLFWwindow *window, [[maybe_unused]]double x_offset, double y_offset){
    auto* camera = static_cast<Engines::Graphics::Camera*>(glfwGetWindowUserPointer(window));
    if(camera) camera->processMouseScroll(static_cast<float>(y_offset));
  }
}