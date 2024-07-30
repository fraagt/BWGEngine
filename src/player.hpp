#pragma once

#include <utility>

#include "object.hpp"
#include "utils/utils.hpp"

class Player : public Object {
public:
    explicit Player(std::shared_ptr<Camera> camera)
            : m_pCamera(std::move(camera)) {
        m_pCamera->transform.rotation.y = -90.0f;
    }

    void update(const Input &input, const Time &time) override {

        auto scroll = input.getScroll();
        m_pCamera->transform.rotation.y -= scroll.y;

        auto cursorOffset = input.getCursorOffset();
        if (glm::length(cursorOffset) > 0.0f) {
            cursorOffset *= sensitivity * time.deltaTime;
            m_pCamera->transform.rotation.x -= cursorOffset.y;
            m_pCamera->transform.rotation.y += cursorOffset.x;
//            check_pitch();
        }


        auto movementDirection = glm::zero<glm::vec3>();
        if (input.getKey(GLFW_KEY_W)) {
            movementDirection.z += 1.0f;
        }
        if (input.getKey(GLFW_KEY_S)) {
            movementDirection.z -= 1.0f;
        }
        if (input.getKey(GLFW_KEY_A)) {
            movementDirection.x -= 1.0f;
        }
        if (input.getKey(GLFW_KEY_D)) {
            movementDirection.x += 1.0f;
        }

        if (glm::length(movementDirection) > 0.0f) {
            glm::vec3 front = m_pCamera->transform.getFront();
            glm::vec3 right = m_pCamera->transform.getRight();
            movementDirection = glm::normalize(movementDirection) * speed * time.deltaTime;

            m_pCamera->transform.position += movementDirection.x * right + movementDirection.z * front;
        }

        if (input.getKey(GLFW_KEY_LEFT_SHIFT)) {
            speed *= 1.0f + time.deltaTime;
        }
        if (input.getKey(GLFW_KEY_LEFT_CONTROL)) {
            speed /= 1.0f + time.deltaTime;
        }
    }

private:
    void check_pitch() {
        m_pCamera->transform.rotation.x = glm::clamp(m_pCamera->transform.rotation.x, -89.9f, 89.9f);
    }

private:
    std::shared_ptr<Camera> m_pCamera;
    glm::vec3 target{};
    float speed = 15.0f;
    float sensitivity = 100.0f;
};