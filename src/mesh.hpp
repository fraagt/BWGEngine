#pragma once

#include <vector>
#include "utils/GLCommon.hpp"

class Mesh {
public:
    typedef struct {
        glm::vec3 position;
        glm::vec3 normal;

    } Vertex;

    Mesh(const std::vector<Vertex> &vertices, const std::vector<GLuint> &indices)
            : m_vertices(vertices), m_indices(indices) {

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);

        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *) offsetof(Vertex, normal));

        glBindVertexArray(0);

        m_mode = GL_TRIANGLES;
    }

    void draw() {
        glBindVertexArray(m_vao);
        glDrawElements(m_mode, m_indices.size(), GL_UNSIGNED_INT, (GLvoid *) 0);
        glBindVertexArray(0);
    }

    void draw(GLuint start, GLsizei count) {
        glBindVertexArray(m_vao);
        glDrawElements(m_mode, count, GL_UNSIGNED_INT, (GLvoid *) (start * sizeof(GLuint)));
        glBindVertexArray(0);
    }

private:
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;

    GLuint m_vbo;
    GLuint m_vao;
    GLuint m_ebo;
    GLenum m_mode;
};