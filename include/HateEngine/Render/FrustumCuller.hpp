#pragma once
#include <glm/glm.hpp>
#include <array>

class FrustumCuller {

public:
    enum class Planes { Left = 0, Right, Bottom, Top, Near, Far, Count };

    struct Plane {
        glm::vec3 normal;
        float distance;
    };

    FrustumCuller() = default;

    explicit FrustumCuller(const glm::mat4& projectionView);

    void update(const glm::mat4& projectionView);
    bool isBoxVisible(const glm::vec3& minPoint, const glm::vec3& maxPoint) const;

private:
    std::array<Plane, static_cast<size_t>(Planes::Count)> m_planes;
};
