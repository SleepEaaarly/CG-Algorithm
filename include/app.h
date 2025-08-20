#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "resource_manager.h"
#include "camera.h"


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
    ~App() {
        if (window) {
            glfwDestroyWindow(window);
            window = nullptr;
        }
        glfwTerminate();
    }

    // glfw: whenever the window size changed (by OS or user resize) this callback function executes
    // ---------------------------------------------------------------------------------------------
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        glViewport(0, 0, width, height);
    }

    // glfw: whenever the mouse moves, this callback is called
    // -------------------------------------------------------
    static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        ResourceManager::getInstance().getMainCamera().ProcessMouseMovement(xoffset, yoffset);
    }

    // glfw: whenever the mouse scroll wheel scrolls, this callback is called
    // ----------------------------------------------------------------------
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
        ResourceManager::getInstance().getMainCamera().ProcessMouseScroll(static_cast<float>(yoffset));
    }
    
    static void processInput(GLFWwindow *window) {
        auto main_camera = ResourceManager::getInstance().getMainCamera();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            main_camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            main_camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            main_camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            main_camera.ProcessKeyboard(RIGHT, deltaTime);
    }

public:
    void init(unsigned int width, unsigned int height, const std::string& window_title = "CG-Algorithm") {
        SCR_WIDTH = width;
        SCR_HEIGHT = height;
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

        // glfw window creation
        // --------------------
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, window_title.c_str(), NULL, NULL);
        if (window == NULL) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            assert(false && "Failed to create GLFW window");
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            assert(false && "Failed to initialize GLAD");
        }

        // configure global opengl state
        // -----------------------------
        glEnable(GL_DEPTH_TEST);
    }

    void update() {
        while (!glfwWindowShouldClose(window)) {
            // per-frame time logic
            // --------------------
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // input
            processInput(window);

            // render
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // update before rendering
            ResourceManager::getInstance().updateGameObjects();
            ResourceManager::getInstance().updateRenderPasses();

            // render all
            ResourceManager::getInstance().render();

            // swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
};

