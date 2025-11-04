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
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), this->aspectRatio(), 0.1f, 100.0f);
    auto shader = std::make_shared<Engines::Graphics::Shader>("shaders/vertex.glsl", "shaders/fragment.glsl");
    this->camera = std::make_unique<Engines::Graphics::Camera>();
    this->camera->setShader(shader)
      .setSpeed(1.0f)
      .build();
      
    glm::mat4 sphere_model_postion = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    auto sphere = Engines::Graphics::GeometryBuilder::createSphere()
            .setRadius(10.0f) 
            .setSectorCount(30)
            .setStackCount(18)
            .setPosition(sphere_model_postion)
            .setColor(glm::vec3(1.0f, 1.0f, 1.0f))
            .setShader(shader)
            .build();

    glm::mat4 alpha_model_position = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, 0.0f, 0.0f));
    auto alpha = Engines::Graphics::GeometryBuilder::createSphere()
            .setRadius(2.0f) 
            .setSectorCount(30)
            .setStackCount(18)
            .setPosition(alpha_model_position)
            .setColor(glm::vec3(1.0f, 0.5f, 0.3f))
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

      sphere->draw();
      alpha->draw();

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

  void App::framebufferSizeCallback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
  }
}