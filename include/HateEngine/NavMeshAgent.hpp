#pragma once
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include <DetourNavMeshQuery.h>

#include "Resources/NavMesh.hpp"

namespace HateEngine {
    class NavMeshAgent {
        dtNavMeshQuery* nav_mesh_query = nullptr;
        std::vector<glm::vec3> points_path;
        uint32_t current_point = 0;

    public:
        glm::vec3 startSearchDistance = {2, 4, 2};
        glm::vec3 endSearchDistance = {2, 4, 2};
        uint16_t max_path_count = 256;

        NavMeshAgent(NavMesh* nav_mesh, uint16_t max_polys);
        ~NavMeshAgent();

        bool computePath(glm::vec3 start, glm::vec3 end);
        std::vector<glm::vec3> getPointsPath();
        // glm::vec3 getNextNearestPoint(glm::vec3 position);
        glm::vec3 getDirectionByPosition(glm::vec3 position, float threshold = 0.1f);
        bool isEndOfPath();
    };
} // namespace HateEngine
