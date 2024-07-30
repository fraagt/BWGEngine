#include <fstream>
#include <iostream>
#include <sstream>
#include "objconverter.hpp"

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        *result++ = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

OBJConverter::OBJConverter(const char *filePath) {
    m_nFaces = 0;
    std::string content;
    std::ifstream file;
    file.open(filePath);
    if (!file.is_open())
        throw std::ios_base::failure("File is not open");

    std::string line;
    while (std::getline(file, line)) {

        if (line.empty())
            continue;

        auto entries = split(line, ' ');

        if (entries[0] == "#")
            continue;

        if (entries[0] == "mtllib")
            continue;

        if (entries[0] == "o") {
            m_name = entries[1];
            continue;
        }

        if (entries[0] == "v") {
            for (int i = 1; i < entries.size(); ++i) {
                m_vertices.push_back(std::stof(entries[i]));
            }
            continue;
        }

        if (entries[0] == "vn") {
            for (int i = 1; i < entries.size(); ++i) {
                m_normals.push_back(std::stof(entries[i]));
            }
            continue;
        }

        if (entries[0] == "s") {
            continue;
        }

        if (entries[0] == "f") {
            ++m_nFaces;
            for (int i = 1; i < entries.size(); ++i) {
                auto indices = split(entries[i], '/');
                m_indices.push_back(std::stoi(indices[0]) - 1);
                m_normalIndices.push_back(std::stoi(indices[2]) - 1);
            }
            continue;
        }
    }
    file.close();
}

OBJConverter::~OBJConverter() {

}

std::vector<float> OBJConverter::getVertices() {
    return m_vertices;
}

std::vector<float> OBJConverter::getNormals() {
    return m_normals;
}

std::vector<uint32_t> OBJConverter::getIndices() {
    return m_indices;
}

std::vector<uint32_t> OBJConverter::getNormalIndices() {
    return m_normalIndices;
}

uint32_t OBJConverter::getNFaces() {
    return m_nFaces;
}
