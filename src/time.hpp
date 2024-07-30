#pragma once

#include "utils/GLCommon.hpp"

struct Time {
    float currentTime;
    float lastTime;
    float deltaTime;

    void update() {
        currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
    }
};