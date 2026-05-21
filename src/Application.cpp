#include "Application.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <cmath>

const unsigned int SCR_WIDTH = 1800;
const unsigned int SCR_HEIGHT = 1200;

Application::Application()
    : window(nullptr), camera(), mouse(camera) {}

Application::~Application() {
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

int Application::run() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Kaczka", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glClearColor(0.08f, 0.10f, 0.14f, 1.0f);

    cubeMap = std::make_unique<CubeMap>("../lib/textures/forest.jpg",
                                        "../lib/textures/rocks.jpg");
    skybox = std::make_unique<SkyboxRenderer>(cubeMap->id());
    waterRenderer = std::make_unique<WaterRenderer>(WaterSimulation::N, cubeMap->id());
    duck = std::make_unique<DuckRenderer>("../lib/duck/duck.txt",
                                          "../lib/duck/ducktex.jpg");

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        float aspect = display_h > 0 ? static_cast<float>(display_w) / display_h : 1.0f;

        // Kaczka
        const float speed = 0.5f;
        float u = std::fmod(static_cast<float>(glfwGetTime()) * speed,
                            static_cast<float>(spline.segmentCount()));
        glm::vec3 duckPos = spline.position(u);
        glm::vec3 tang = spline.tangent(u);

        // Symulacja wody
        water.maybeDropRain();
        int duckJ = static_cast<int>((duckPos.x + 1.0f) * 0.5f * (WaterSimulation::N - 1));
        int duckI = static_cast<int>((duckPos.z + 1.0f) * 0.5f * (WaterSimulation::N - 1));
        water.disturb(duckI, duckJ, 0.02f);
        water.step();
        water.uploadNormals(waterRenderer->getNormalTexture());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        skybox->draw(camera, aspect);
        waterRenderer->draw(camera, aspect);


        glm::vec2 dir2 = glm::normalize(glm::vec2(tang.x, tang.z));
        float angle = std::atan2(dir2.y, -dir2.x);
        glm::mat4 duckModel = glm::translate(glm::mat4(1.0f), duckPos)
                            * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0))
                            * glm::scale(glm::mat4(1.0f), glm::vec3(0.002f));
        duck->draw(camera, aspect, duckModel, lightPos);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}

void Application::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) glViewport(0, 0, width, height);
}

void Application::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) app->mouse.onMouseButton(button, action);
}

void Application::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app) app->mouse.onCursorPos(xpos, ypos);
}
