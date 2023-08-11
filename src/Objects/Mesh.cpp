#include "Mesh.h"

using namespace Old3DEngine;
#include <iostream>
Mesh::~Mesh() {
    delete [] verticies;
    delete [] indicies;
    delete [] normals;
}
