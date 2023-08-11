#include "CubeMesh.h"

using namespace Old3DEngine;

CubeMesh::CubeMesh() {
    verticies.reserve(24);
    indicies.reserve(36);
    this->verticies =  {
            -0.5, -0.5,  0.5,
             0.5, -0.5,  0.5,
             0.5,  0.5,  0.5,
            -0.5,  0.5,  0.5,
            -0.5, -0.5, -0.5,
             0.5, -0.5, -0.5,
             0.5,  0.5, -0.5,
            -0.5,  0.5, -0.5,
    };
    this->indicies = {
        0, 1, 2,
        2, 3, 0,
        1, 5, 6,
        6, 2, 1,
        4, 7, 6,
        6, 5, 4,
        4, 0, 3,
        3, 7, 4,
        3, 2, 6,
        6, 7, 3,
        0, 4, 5,
        5, 1, 0

    };
}