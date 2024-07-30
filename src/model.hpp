#pragma once

#include <memory>
#include <vector>
#include "shader.hpp"
#include "input.hpp"
#include "mesh.hpp"
#include "transform.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "time.hpp"

class Model : public Object {
public:
    Model();

    void update(const Input &input, const Time &time) override;

    void draw(const Camera &camera) override;

private:
    std::unique_ptr<Transform> m_pTransform;
    std::unique_ptr<Shader> m_pShader;
    std::unique_ptr<Mesh> m_pMesh;

    glm::vec3 m_lightPosition;
    glm::vec3 m_lightColor;
    glm::vec3 m_objectColor;
};