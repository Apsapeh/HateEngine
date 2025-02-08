#include <HateEngine/Render/FrustumCuller.hpp>

FrustumCuller::FrustumCuller(const glm::mat4& projectionView) {
    update(projectionView);
}

void FrustumCuller::update(const glm::mat4& projectionView) {
    std::array<Plane, static_cast<size_t>(Planes::Count)> planes;

    // Левая плоскость
    planes[static_cast<size_t>(Planes::Left)].normal.x =
            projectionView[0][3] + projectionView[0][0];
    planes[static_cast<size_t>(Planes::Left)].normal.y =
            projectionView[1][3] + projectionView[1][0];
    planes[static_cast<size_t>(Planes::Left)].normal.z =
            projectionView[2][3] + projectionView[2][0];
    planes[static_cast<size_t>(Planes::Left)].distance =
            projectionView[3][3] + projectionView[3][0];

    // Правая плоскость
    planes[static_cast<size_t>(Planes::Right)].normal.x =
            projectionView[0][3] - projectionView[0][0];
    planes[static_cast<size_t>(Planes::Right)].normal.y =
            projectionView[1][3] - projectionView[1][0];
    planes[static_cast<size_t>(Planes::Right)].normal.z =
            projectionView[2][3] - projectionView[2][0];
    planes[static_cast<size_t>(Planes::Right)].distance =
            projectionView[3][3] - projectionView[3][0];

    // Нижняя плоскость
    planes[static_cast<size_t>(Planes::Bottom)].normal.x =
            projectionView[0][3] + projectionView[0][1];
    planes[static_cast<size_t>(Planes::Bottom)].normal.y =
            projectionView[1][3] + projectionView[1][1];
    planes[static_cast<size_t>(Planes::Bottom)].normal.z =
            projectionView[2][3] + projectionView[2][1];
    planes[static_cast<size_t>(Planes::Bottom)].distance =
            projectionView[3][3] + projectionView[3][1];

    // Верхняя плоскость
    planes[static_cast<size_t>(Planes::Top)].normal.x = projectionView[0][3] - projectionView[0][1];
    planes[static_cast<size_t>(Planes::Top)].normal.y = projectionView[1][3] - projectionView[1][1];
    planes[static_cast<size_t>(Planes::Top)].normal.z = projectionView[2][3] - projectionView[2][1];
    planes[static_cast<size_t>(Planes::Top)].distance = projectionView[3][3] - projectionView[3][1];

    // Ближняя плоскость
    planes[static_cast<size_t>(Planes::Near)].normal.x =
            projectionView[0][3] + projectionView[0][2];
    planes[static_cast<size_t>(Planes::Near)].normal.y =
            projectionView[1][3] + projectionView[1][2];
    planes[static_cast<size_t>(Planes::Near)].normal.z =
            projectionView[2][3] + projectionView[2][2];
    planes[static_cast<size_t>(Planes::Near)].distance =
            projectionView[3][3] + projectionView[3][2];

    // Дальняя плоскость
    planes[static_cast<size_t>(Planes::Far)].normal.x = projectionView[0][3] - projectionView[0][2];
    planes[static_cast<size_t>(Planes::Far)].normal.y = projectionView[1][3] - projectionView[1][2];
    planes[static_cast<size_t>(Planes::Far)].normal.z = projectionView[2][3] - projectionView[2][2];
    planes[static_cast<size_t>(Planes::Far)].distance = projectionView[3][3] - projectionView[3][2];

    // Нормализуем все плоскости
    for (auto& plane: planes) {
        float length = glm::length(plane.normal);
        plane.normal /= length;
        plane.distance /= length;
    }

    m_planes = planes;
}

// In AABB
bool FrustumCuller::isBoxVisible(const glm::vec3& minPoint, const glm::vec3& maxPoint) const {
    // Для каждой плоскости усечения
    for (const auto& plane: m_planes) {
        // Находим самую дальнюю точку куба относительно нормали плоскости
        glm::vec3 p(
                plane.normal.x >= 0 ? maxPoint.x : minPoint.x,
                plane.normal.y >= 0 ? maxPoint.y : minPoint.y,
                plane.normal.z >= 0 ? maxPoint.z : minPoint.z
        );

        // Если самая дальняя точка находится за плоскостью - весь куб невидим
        if (glm::dot(plane.normal, p) + plane.distance < 0) {
            return false;
        }
    }

    return true;
}
