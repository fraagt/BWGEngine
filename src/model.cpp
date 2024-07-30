#include <string>
#include "model.hpp"
#include "utils/files.hpp"
#include "utils/OBJConverter/objconverter.hpp"
#include "input.hpp"
#include "tiny_obj_loader.h"
#include "utils/assets.hpp"
#include "camera.hpp"
#include "utils/utils.hpp"

Model::Model() {
    std::string vertexShaderPath = Folders::Assets + R"(\shaders\default.vert)";
    std::string fragmentShaderPath = Folders::Assets + R"(\shaders\default.frag)";

    std::string vertexShaderCode = utils::readFile(vertexShaderPath.c_str());
    std::string fragmentShaderCode = utils::readFile(fragmentShaderPath.c_str());

    m_pShader = std::make_unique<Shader>(vertexShaderCode.c_str(), fragmentShaderCode.c_str());

    std::string fileName = Folders::Assets + R"(\models\cube.obj)";

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string error;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &error, fileName.c_str())) {
        std::cout << error << std::endl;
    }

    std::vector<Mesh::Vertex> vertices;
    std::vector<GLuint> indices;
    for (GLuint i = 0; i < shapes[0].mesh.indices.size(); ++i) {
        const auto &index = shapes[0].mesh.indices[i];

        vertices.push_back({{
                                    attrib.vertices[(index.vertex_index * 3)],
                                    attrib.vertices[(index.vertex_index * 3) + 1],
                                    attrib.vertices[(index.vertex_index * 3) + 2],
                            },
                            {
                                    attrib.normals[(index.normal_index * 3)],
                                    attrib.normals[(index.normal_index * 3) + 1],
                                    attrib.normals[(index.normal_index * 3) + 2],
                            }});

        indices.push_back(i);
    }
    m_pMesh = std::make_unique<Mesh>(vertices, indices);

    auto position = glm::vec3(0.0f, 0.0f, -10.0f);
    auto rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    auto scale = glm::vec3(1.0f, 1.0f, 1.0f);

    m_pTransform = std::make_unique<Transform>(position, rotation, scale);
    m_objectColor = glm::vec3(0.059f, 0.42f, 0.722f);
    m_lightPosition = glm::vec3(3.0f, 5.0f, 5.0f);
    m_lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
}

bool isBypass = false;

void Model::update(const Input &input, const Time &time) {
    auto &position = m_pTransform->position;
    auto &rotation = m_pTransform->rotation;
    auto &scale = m_pTransform->scale;

    auto speed = 50.0f;

    if (input.getKeyDown(GLFW_KEY_B)) {
        isBypass = !isBypass;
    }
    if (input.getKey(GLFW_KEY_Y) || isBypass) {
        rotation.y += time.deltaTime * speed;
    }
    if (input.getKey(GLFW_KEY_X) || isBypass) {
        rotation.x += time.deltaTime * speed;
    }
    if (input.getKey(GLFW_KEY_Z) || isBypass) {
        rotation.z += time.deltaTime * speed;
    }

}

std::vector<glm::vec3> colors = {
        glm::vec3(1.0f, 0.0f, 0.0f),  // Red
        glm::vec3(0.0f, 1.0f, 0.0f),  // Green
        glm::vec3(0.0f, 0.0f, 1.0f),  // Blue
        glm::vec3(1.0f, 1.0f, 0.0f),  // Yellow
        glm::vec3(0.0f, 1.0f, 1.0f),  // Cyan
        glm::vec3(1.0f, 0.0f, 1.0f),  // Magenta
        glm::vec3(1.0f, 0.5f, 0.0f),  // Orange
        glm::vec3(0.5f, 0.0f, 1.0f),  // Purple
        glm::vec3(0.0f, 1.0f, 0.5f),  // Teal
        glm::vec3(0.5f, 1.0f, 0.0f),  // Lime
        glm::vec3(1.0f, 0.0f, 0.5f),  // Pink
        glm::vec3(0.5f, 0.5f, 0.5f)   // Gray
};

void Model::draw(const Camera &camera) {
    auto model = m_pTransform->getMatrix();
    auto transform = camera.getProjection() * camera.getView() * model;

    m_pShader->use();
    m_pShader->setMatrix4fv("transform", transform);
    m_pShader->setMatrix4fv("model", model);
    m_pShader->setVec3f("viewPosition", camera.transform.position);
    m_pShader->setVec3f("lightPosition", m_lightPosition);
    m_pShader->setVec3f("lightColor", m_lightColor);

//    m_pMesh->draw();


    for (GLuint i = 0; i < 12; ++i) {
        m_pShader->setVec3f("objectColor", colors[i] *  0.75f);
        m_pMesh->draw(i * 3, 3);
    }
}
