#ifndef GAME_APPLICATION_H
#define GAME_APPLICATION_H

#include "camera.h"
#include "scene.h"

#include <GLFW/glfw3.h>

#include <string>

class Application {
public:
    Application(int width, int height, const std::string& title);
    ~Application();

    bool initialize();
    void run();

private:
    int width;
    int height;
    std::string title;

    GLFWwindow* window;

    Camera camera;
    Scene scene;

    float lastFrameTime;
    float lastMouseX;
    float lastMouseY;
    bool firstMouseSample;

    static Application* instance;

    void processInput(float dt);

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void mouseCallback(GLFWwindow* window, double xPos, double yPos);
    static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};

#endif
