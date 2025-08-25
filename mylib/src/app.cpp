#include "app.h"
#include "resource_manager.h"
#include "game_object.h"
#include "render_pass.h"
#include <iostream>

float App::lastX = 0.f;
float App::lastY = 0.f;
bool App::firstMouse = true;
float App::deltaTime = 0.f;
float App::lastFrame = 0.f;

App::~App() {
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

void App::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina
    // displays.
    glViewport(0, 0, width, height);
}

void App::mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset =
        lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    ResourceManager::getInstance().getMainCamera().ProcessMouseMovement(
        xoffset, yoffset);
}

void App::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    ResourceManager::getInstance().getMainCamera().ProcessMouseScroll(
        static_cast<float>(yoffset));
}

void App::processInput(GLFWwindow *window) {
    Camera& main_camera = ResourceManager::getInstance().getMainCamera();
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

void App::init(unsigned int width, unsigned int height,
               const std::string &window_title) {
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
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, window_title.c_str(), NULL,
                              NULL);
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

void App::run() {
    ResourceManager::getInstance().initResources();

    for (auto pre_pass : ResourceManager::getInstance().getAllPreRenderPasses()) {
        glClearColor(0.f, 0.f, 0.f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        pre_pass->render();
    }

    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

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
        for (auto obj : ResourceManager::getInstance().getAllGameObjects()) {
            obj->update();
        }

        // render all
        for (auto pass : ResourceManager::getInstance().getAllRenderPasses()) {
            pass->update();
            pass->render();
        }

        // swap buffers and poll IO events (keys pressed/released, mouse moved
        // etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

const unsigned int App::getWidth() const { return SCR_WIDTH; }

const unsigned int App::getHeight() const { return SCR_HEIGHT; }

const float App::getCurrentTime() const { return static_cast<float>(glfwGetTime()); }
