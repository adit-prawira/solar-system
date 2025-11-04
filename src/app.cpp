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
    auto shader = std::make_shared<Engines::Graphics::Shader>("shaders/vertex.glsl", "shaders/fragment.glsl");

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
            .setRadius(5.0f) 
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
      glm::mat4 projection = glm::perspective(glm::radians(45.0f), this->aspectRatio(), 0.1f, 100.0f);
      glm::mat4 view = glm::lookAt(this->camera_position, this->camera_position + this->camera_front, glm::vec3(0, 1, 0));

      shader->setMat4("view", view);
      shader->setMat4("projection", projection);
      shader->setVec3("light_position", this->light_position);
      shader->setVec3("view_position", this->camera_position);
      shader->setVec3("light_color", glm::vec3(1.0f));

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

    glm::vec3 forward = glm::normalize(glm::vec3(sin(glm::radians(camera_angle)), 0.0f, -cos(glm::radians(camera_angle))));
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      this->camera_position.x -= this->camera_speed;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      this->camera_position.x += this->camera_speed;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      this->camera_position.y += this->camera_speed;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      this->camera_position.y -= this->camera_speed;

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
      this->camera_position.z -= this->camera_speed;
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
      this->camera_position.z += this->camera_speed;

    // --- Rotate camera with arrow keys ---
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        this->camera_angle -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        this->camera_angle += 1.0f;
    
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
      this->camera_position = this->initial_camera_position;
      this->camera_front = this->initial_camera_front;
      this->camera_angle = this->initial_camera_angle;
    }

    // --- Compute direction the camera is facing based on angle ---
    float angle_rad = glm::radians(this->camera_angle);
    this->camera_front = glm::normalize(glm::vec3(
        sin(angle_rad), 0.0f, -cos(angle_rad)
    ));
  }

  void App::framebufferSizeCallback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
  }
}