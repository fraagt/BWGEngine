#pragma once

#include <vector>
#include <map>
#include <queue>
#include <chrono>
#include "utils/GLCommon.hpp"
#include "model.hpp"
#include "input.hpp"
#include "camera.hpp"
#include "time.hpp"

class Application {
public:
    Application();

    ~Application();

    void run();

public:
    void handleKey(int keyCode, int action);

    void handleScroll(float x, float y);

    void handleCursorPosition(double x, double y);

private:
    std::vector<std::unique_ptr<Object>> m_pObjects;
    std::shared_ptr<Camera> m_pCamera;
    std::unique_ptr<Input> m_pInput;
    std::unique_ptr<Time> m_pTime;

    GLFWwindow *m_pWindow;
    int m_width;
    int m_height;

    std::chrono::time_point<std::chrono::high_resolution_clock> m_startedAt;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_lastUpdate;
};
