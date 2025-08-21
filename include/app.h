#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cassert>
#include <string>
#include "singleton.h"

class App : public Singleton<App> {
    friend class Singleton<App>;
private:
    unsigned int SCR_WIDTH;
    unsigned int SCR_HEIGHT;
    GLFWwindow* window;

    static float lastX;
    static float lastY;
    static bool firstMouse;
    static float deltaTime;
    static float lastFrame;

    App() = default;
    ~App();

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    // glfw: whenever the mouse moves, this callback is called
    // -------------------------------------------------------
    static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    // ----------------------------------------------------------------------
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    
    static void processInput(GLFWwindow *window);

public:
    void init(unsigned int width, unsigned int height, const std::string& window_title = "CG-Algorithm");

    void run();

    const unsigned int getWidth() const;
    const unsigned int getHeight() const;
    const float getCurrentTime() const;

};

