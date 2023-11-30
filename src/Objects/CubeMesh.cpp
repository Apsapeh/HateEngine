#include <Old3DEngine/Objects/CubeMesh.hpp>

using namespace Old3DEngine;

CubeMesh::CubeMesh() {
    verticies.reserve(24);
    indicies.reserve(36);

    setSize(1, 1, 1);

    this->indicies = {
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
            11, 12, 13, 14, 15, 16, 17, 18, 19,
            20, 21, 22, 23, 24, 25, 26, 27, 28,
            29, 30, 31, 32, 33, 34, 35
    };

    this->normals = {
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,

            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,
            1, 0, 0,

            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
            0, 0, -1,

            -1, 0, 0,
            -1, 0, 0,
            -1, 0, 0,
            -1, 0, 0,
            -1, 0, 0,
            -1, 0, 0,

            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,

            0, -1, 0,
            0, -1, 0,
            0, -1, 0,
            0, -1, 0,
            0, -1, 0,
            0, -1, 0,
    };

    this->UV = {
            0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0,
            0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0,
            0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0,
            0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0,
            0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0,
            0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0,
    };
}

void CubeMesh::setSize(float width, float height, float length) {
    float x = 0.5f * width;
    float y = 0.5f * height;
    float z = 0.5f * length;

    this->verticies = {
            -x, -y,  z,
            x, -y,  z,
            x,  y,  z,

            x,  y,  z,
            -x,  y,  z,
            -x, -y,  z,

            x, -y,  z,
            x, -y, -z,        //5
            x,  y, -z,        //6

            x,  y, -z,        //6
            x,  y,  z,        //2
            x, -y,  z,        //1

            -x, -y, -z,       //4
            -x,  y, -z,       //7
            x,  y, -z,        //6

            x,  y, -z,        //6
            x, -y, -z,        //5
            -x, -y, -z,       //4

            -x, -y, -z,       //4
            -x, -y,  z,       //0
            -x,  y,  z,       //3

            -x,  y,  z,       //3
            -x,  y, -z,       //7
            -x, -y, -z,       //4

            -x,  y,  z,       //3
            x,  y,  z,        //2
            x,  y, -z,        //6

            x,  y, -z,        //6
            -x,  y, -z,       //7
            -x,  y,  z,       //3

            -x, -y,  z,       //0
            -x, -y, -z,       //4
            x, -y, -z,        //5

            x, -y, -z,        //5
            x, -y,  z,        //1
            -x, -y,  z,       //0
    };

    float tex_coord_x = 0;
    float tex_coord_y = 0;
}
