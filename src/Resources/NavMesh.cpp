#include <HateEngine/Resources/NavMesh.hpp>
#include <HateEngine/Log.hpp>
#include <cstdio>
#include <vector>
#include <cstdint>
#include <cstring>
#include "DetourNavMesh.h"
#include "DetourStatus.h"
using namespace HateEngine;

static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';
static const int NAVMESHSET_VERSION = 1;

struct NavMeshSetHeader {
    int magic;
    int version;
    int numTiles;
    dtNavMeshParams params;
};

struct NavMeshTileHeader {
    dtTileRef tileRef;
    int dataSize;
};

NavMesh::NavMesh(const std::string& file_name) {
    // This code just copied from RecastDemo/Sample.cpp@Sample::loadAll
    FILE* fp = fopen(file_name.c_str(), "rb");
    if (!fp) {
        HATE_ERROR_F("NavMesh: Could not open file: %s", file_name.c_str());
        return;
    }

    // Read header.
    NavMeshSetHeader header;
    size_t readLen = fread(&header, sizeof(NavMeshSetHeader), 1, fp);
    if (readLen != 1) {
        fclose(fp);
        HATE_ERROR_F("NavMesh: Could not read file: %s", file_name.c_str());
        return;
    }
    if (header.magic != NAVMESHSET_MAGIC) {
        fclose(fp);
        HATE_ERROR_F("NavMesh: Could not read file: %s", file_name.c_str());
        return;
    }
    if (header.version != NAVMESHSET_VERSION) {
        fclose(fp);
        HATE_ERROR_F("NavMesh: Could not read file: %s", file_name.c_str());
        return;
    }

    dtNavMesh* mesh = dtAllocNavMesh();
    if (!mesh) {
        fclose(fp);
        HATE_ERROR_F("NavMesh: Could not read file: %s", file_name.c_str());
        return;
    }
    dtStatus status = mesh->init(&header.params);
    if (dtStatusFailed(status)) {
        fclose(fp);
        HATE_ERROR_F("NavMesh: Could not read file: %s", file_name.c_str());
        return;
    }

    // Read tiles.
    for (int i = 0; i < header.numTiles; ++i) {
        NavMeshTileHeader tileHeader;
        readLen = fread(&tileHeader, sizeof(tileHeader), 1, fp);
        if (readLen != 1) {
            fclose(fp);
            HATE_ERROR_F("NavMesh: Could not read file: %s", file_name.c_str());
            return;
        }

        if (!tileHeader.tileRef || !tileHeader.dataSize)
            break;

        unsigned char* data = (unsigned char*) dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
        if (!data)
            break;
        memset(data, 0, tileHeader.dataSize);
        readLen = fread(data, tileHeader.dataSize, 1, fp);
        if (readLen != 1) {
            dtFree(data);
            fclose(fp);
            HATE_ERROR_F("NavMesh: Could not read file: %s", file_name.c_str());
            return;
        }

        dtStatus status =
                mesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
        if (dtStatusFailed(status)) {
            dtFree(data);
            fclose(fp);
            HATE_ERROR_F("NavMesh: Could not read file: %s", file_name.c_str());
            return;
        }
    }

    fclose(fp);

    this->nav_mesh = mesh;
    this->is_loaded = true;
}

NavMesh::~NavMesh() {
    if (this->nav_mesh) {
        dtFreeNavMesh(this->nav_mesh);
        this->nav_mesh = nullptr;
    }
    this->is_loaded = false;
}


dtNavMesh* NavMesh::getNavMesh() {
    return this->nav_mesh;
}
