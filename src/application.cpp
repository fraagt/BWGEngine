#include <iostream>
#include <thread>
#include "application.hpp"
#include "player.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void keyboard_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto *handler = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (handler)
        handler->handleKey(key, action);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    auto *handler = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (handler)
        handler->handleScroll(static_cast<float>(xoffset), static_cast<float>(yoffset));
}

void mouse_position_callback(GLFWwindow *window, double x, double y) {
    auto *handler = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
    if (handler)
        handler->handleCursorPosition(x, y);
}

Application::Application() {
    m_width = 1280;
    m_height = 720;

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    m_pWindow = glfwCreateWindow(m_width, m_height, "SpinningSpheres", NULL, NULL);
    if (m_pWindow == nullptr) {
        std::cout << "Failed to create GLFW m_window" << '\n';
        glfwTerminate();
        throw EXIT_FAILURE;
    }

    glfwSetWindowUserPointer(m_pWindow, reinterpret_cast<void *>(this));
    glfwMakeContextCurrent(m_pWindow);

    if (!gladLoaderLoadGL()) {
        std::cout << "Failed to load GLAD" << '\n';
        throw EXIT_FAILURE;
    }

    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glViewport(0, 0, m_width, m_height);
    glfwSetKeyCallback(m_pWindow, keyboard_callback);
    glfwSetScrollCallback(m_pWindow, scroll_callback);
    glfwSetCursorPosCallback(m_pWindow, mouse_position_callback);

    m_pInput = std::make_unique<Input>();
    glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(m_pWindow, static_cast<double>(m_width) / 2.0, static_cast<double>(m_height) / 2.0);
    double cursorX, cursorY;
    glfwGetCursorPos(m_pWindow, &cursorX, &cursorY);
    m_pInput->setFirstCursorPos(cursorX, cursorY);

    m_pCamera = std::make_shared<Camera>();
    m_pTime = std::make_unique<Time>();

    m_pObjects.emplace_back(std::make_unique<Model>());
    m_pObjects.emplace_back(std::make_unique<Player>(m_pCamera));


    m_pCamera->fov = 45.0f;
    m_pCamera->m_plane = glm::vec2(m_width, m_height);
    m_pCamera->m_zNear = 0.1f;
    m_pCamera->m_zFar = 100.0f;

    m_startedAt = std::chrono::high_resolution_clock::now();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

Application::~Application() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_pWindow);
    glfwTerminate();
}

void Application::run() {
    m_lastUpdate = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(m_pWindow)) {
        glfwPollEvents();

        m_pInput->update();
        m_pTime->update();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

//        ImGui::ShowDemoWindow();

        glClearColor(0.10f, 0.15f, 0.20f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (m_pInput->getKeyDown(GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(m_pWindow, GL_TRUE);

        for (const auto &object: m_pObjects)
            object->update(*m_pInput, *m_pTime);

        m_lastUpdate = std::chrono::high_resolution_clock::now();

        for (const auto &object: m_pObjects)
            object->draw(*m_pCamera);

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cout << "ERROR " << std::to_string(err) << '\n';
        }

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_pWindow);
    }
}

void Application::handleKey(int keyCode, int action) {
    m_pInput->handleKeyEvent(keyCode, action);
}

void Application::handleScroll(float x, float y) {
    m_pInput->handleScroll(x, y);
}

void Application::handleCursorPosition(double x, double y) {
    m_pInput->handleCursorPosition(x, y);
}
