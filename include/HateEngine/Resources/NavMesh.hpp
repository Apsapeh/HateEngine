#pragma once

#include <DetourNavMesh.h>
#include <string>

#include "Resource.hpp"

namespace HateEngine {
    class NavMesh : public Resource {
        dtNavMesh* nav_mesh = nullptr;

    public:
        NavMesh(const std::string& file_name);

        ~NavMesh();

        dtNavMesh* getNavMesh();
    };
} // namespace HateEngine
