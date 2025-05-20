#include <HateEngine/NavMeshAgent.hpp>
#include <HateEngine/Log.hpp>
#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"
#include "DetourStatus.h"

using namespace HateEngine;

NavMeshAgent::NavMeshAgent(NavMesh* nav_mesh, uint16_t max_polys) {
    nav_mesh_query = dtAllocNavMeshQuery();
    if (!nav_mesh_query) {
        HATE_ERROR("NavMeshAgent: Could not allocate NavMeshQuery");
        return;
    }
    dtStatus status = nav_mesh_query->init(nav_mesh->getNavMesh(), 2048);
    if (status != DT_SUCCESS) {
        HATE_ERROR("NavMeshAgent: Could not init NavMeshQuery");
        dtFreeNavMeshQuery(nav_mesh_query);
        return;
    }
}

NavMeshAgent::~NavMeshAgent() {
    if (!nav_mesh_query)
        dtFreeNavMeshQuery(nav_mesh_query);
}

bool NavMeshAgent::computePath(glm::vec3 start, glm::vec3 end) {
    current_point = 0;
    this->points_path.clear();

    if (!nav_mesh_query) {
        HATE_ERROR("NavMeshAgent: NavMeshQuery is not initialized");
        return false;
    }

    dtPolyRef startRef = 1000, endRef = 1000;
    float nearestPoint[3];
    // Filter
    dtQueryFilter filter;
    filter.setIncludeFlags(0xffff);

    dtStatus start_status = nav_mesh_query->findNearestPoly(
            &start[0], &startSearchDistance[0], &filter, &startRef, nearestPoint
    );
    dtStatus end_status = nav_mesh_query->findNearestPoly(
            &end[0], &endSearchDistance[0], &filter, &endRef, nearestPoint
    );

    if (start_status != DT_SUCCESS) {
        HATE_ERROR_F("NavMeshAgent: Could not find nearest poly to start, status: %u", start_status);
        return false;
    }

    if (end_status != DT_SUCCESS) {
        HATE_ERROR_F("NavMeshAgent: Could not find nearest poly to end, status: %u", end_status);
        return false;
    }

    if (!startRef or !endRef) {
        HATE_ERROR("NavMeshAgent: Could not find nearest poly to start or end");
        return false;
    }


    dtPolyRef* path = new dtPolyRef[max_path_count];
    int path_count = 0;
    dtStatus status = nav_mesh_query->findPath(
            startRef, endRef, &start[0], &end[0], &filter, path, &path_count, max_path_count
    );
    if (dtStatusFailed(status)) {
        HATE_ERROR("NavMeshAgent: Could not find path");
        HATE_INFO_F("status: %u", status);
        delete[] path;
        return false;
    }

    glm::vec3* path_points = new glm::vec3[max_path_count + 2];
    unsigned char* straightPathFlags = new unsigned char[max_path_count + 2];
    dtPolyRef* straightPathPolys = new dtPolyRef[max_path_count + 2];
    int straightPathCount = 0;

    status = nav_mesh_query->findStraightPath(
            &start[0], &end[0], path, path_count, (float*) path_points, straightPathFlags,
            straightPathPolys, &straightPathCount, max_path_count
    );

    delete[] path;
    delete[] straightPathFlags;
    delete[] straightPathPolys;

    if (dtStatusFailed(status)) {
        HATE_ERROR_F("NavMeshAgent: Could not find straight path, status: %u", status);
        delete[] path_points;
        return false;
    }


    this->points_path.reserve(straightPathCount);
    std::copy(path_points, path_points + straightPathCount, std::back_inserter(this->points_path));

    delete[] path_points;

    /*for (auto p : this->points_path) {
        HATE_INFO_F("%f, %f, %f", p.x, p.y, p.z);
    }*/
    /*float min_dist = std::numeric_limits<float>::max();
    uint32_t min_index = 0;

    for (uint32_t i = 0; i < this->points_path.size(); ++i) {
        float dist = glm::length(this->points_path[i] - start);
        if (dist < min_dist) {
            min_dist = dist;
            min_index = i;
        }
    }
    this->current_point = min_index;*/

    return true;
}

std::vector<glm::vec3> NavMeshAgent::getPointsPath() {
    return this->points_path;
}


glm::vec3 NavMeshAgent::getDirectionByPosition(glm::vec3 position, float threshold) {
    if (this->points_path.size() == 0) {
        return {0, 0, 0};
    }

    if (glm::distance(this->points_path[this->current_point], position) < threshold) {
        if (this->current_point + 1 < this->points_path.size()) {
            ++this->current_point;
        } else {
            return {0, 0, 0};
        }
    }

    return glm::normalize(this->points_path[this->current_point] - position);
}

bool NavMeshAgent::isEndOfPath() {
    return this->current_point + 1 >= this->points_path.size();
}
