#pragma once

#include <memory>
#include "transform.hpp"
#include "utils/vector_utils.hpp"

struct Camera {
    Transform transform;
    float fov{};
    glm::vec2 m_plane{};
    float m_zNear{};
    float m_zFar{};

    Camera() = default;

    glm::mat4 getProjection() const {
        return glm::perspective(glm::radians(fov), m_plane.x / m_plane.y, m_zNear, m_zFar);
    }

    glm::mat4 getView() const {
        return glm::lookAt(transform.position, transform.position + transform.getFront(), utils::UP);
    }
};