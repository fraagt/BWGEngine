#include <string>
#include <vector>
#include <cstdint>

class OBJConverter {
public:
    typedef struct {
        uint32_t vertex;
        uint32_t texture;
        uint32_t normal;
    } FaceVertex;

    OBJConverter(const char *filePath);

    ~OBJConverter();

    std::vector<float> getVertices();

    std::vector<float> getNormals();

    std::vector<uint32_t> getIndices();

    std::vector<uint32_t> getNormalIndices();

    uint32_t getNFaces();


private:
    std::string m_name;
    std::vector<float> m_vertices;
    std::vector<float> m_normals;
    std::vector<uint32_t> m_indices;
    std::vector<uint32_t> m_normalIndices;
    uint32_t m_nFaces;

};
