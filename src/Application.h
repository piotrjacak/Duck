#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <memory>

#include "Camera.h"
#include "MouseController.h"
#include "WaterRenderer.h"
#include "WaterSimulation.h"
#include "CubeMap.h"
#include "SkyboxRenderer.h"
#include "DuckRenderer.h"
#include "BSpline.h"

class Application {
public:
    Application();
    ~Application();

    int run();

private:
    GLFWwindow* window;
    Camera camera;
    MouseController mouse;
    std::unique_ptr<WaterRenderer> waterRenderer;
    std::unique_ptr<CubeMap> cubeMap;
    std::unique_ptr<SkyboxRenderer> skybox;
    std::unique_ptr<DuckRenderer> duck;
    WaterSimulation water;
    BSpline spline;

    glm::vec3 lightPos = glm::vec3(2.0f, 3.0f, 2.0f);

    void processInput(GLFWwindow* window);

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
};
