#include "application.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Application* Application::instance = nullptr;

Application::Application(int width, int height, const std::string& title)
    : width(width),
      height(height),
      title(title),
      window(nullptr),
      camera(),
      scene(),
      lastFrameTime(0.0f),
      lastMouseX(static_cast<float>(width) * 0.5f),
      lastMouseY(static_cast<float>(height) * 0.5f),
      firstMouseSample(true) {
}

Application::~Application() {
    if (window != nullptr) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
    instance = nullptr;
}

bool Application::initialize() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }

    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);

    instance = this;
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    scene.initialize();
    return true;
}

void Application::run() {
    while (!glfwWindowShouldClose(window)) {
        const float currentTime = static_cast<float>(glfwGetTime());
        const float deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        processInput(deltaTime);
        scene.update(deltaTime);

        glClearColor(0.06f, 0.08f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::mat4 view = camera.getViewMatrix();
        const glm::mat4 projection = glm::perspective(
            glm::radians(camera.getZoom()),
            static_cast<float>(width) / static_cast<float>(height),
            0.1f,
            200.0f);

        scene.render(view, projection, camera.getPosition());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::processInput(float dt) {
    const bool sprint = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    camera.setMovementSpeed(sprint ? 8.0f : 4.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.processKeyboard(CameraDirection::Forward, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.processKeyboard(CameraDirection::Backward, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.processKeyboard(CameraDirection::Left, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.processKeyboard(CameraDirection::Right, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.processKeyboard(CameraDirection::Up, dt);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.processKeyboard(CameraDirection::Down, dt);
    }
}

void Application::framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height) {
    glViewport(0, 0, width, height);
    if (instance != nullptr) {
        instance->width = width;
        instance->height = height;
    }
}

void Application::mouseCallback(GLFWwindow* /*window*/, double xPos, double yPos) {
    if (instance == nullptr) {
        return;
    }

    if (instance->firstMouseSample) {
        instance->lastMouseX = static_cast<float>(xPos);
        instance->lastMouseY = static_cast<float>(yPos);
        instance->firstMouseSample = false;
    }

    const float xOffset = static_cast<float>(xPos) - instance->lastMouseX;
    const float yOffset = instance->lastMouseY - static_cast<float>(yPos);

    instance->lastMouseX = static_cast<float>(xPos);
    instance->lastMouseY = static_cast<float>(yPos);

    instance->camera.processMouseMovement(xOffset, yOffset);
}

void Application::scrollCallback(GLFWwindow* /*window*/, double /*xOffset*/, double yOffset) {
    if (instance != nullptr) {
        instance->camera.processMouseScroll(static_cast<float>(yOffset));
    }
}

void Application::keyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (instance == nullptr) {
        return;
    }

    if (key == GLFW_KEY_L && action == GLFW_PRESS) {
        instance->scene.toggleLantern();
    }
}
