#include <glm/glm.hpp>
#include <vector>

class BoidsModel {
public:
    BoidsModel(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices)
        : vertices(vertices), indices(indices) {}

    const std::vector<glm::vec3>&    getVertices() const { return vertices; }
    const std::vector<unsigned int>& getIndices() const { return indices; }

private:
    std::vector<glm::vec3>    vertices;
    std::vector<unsigned int> indices;
};