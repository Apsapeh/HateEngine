#include <cstdint>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include <HateEngine/Resources/ObjMapModel.hpp>
#include <vector>
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/matrix.hpp"

using namespace HateEngine;

std::vector<std::string> split(std::string str, char delimiter);

ObjMapModel::ObjMapModel(std::string obj_filename, std::string map_file_name){
    std::ifstream file(obj_filename);

    std::string data( 
        (std::istreambuf_iterator<char>(file)),
        (std::istreambuf_iterator<char>())
    );
    //std::cout << data << "\n";
    parseObj(data);
}

ObjMapModel::ObjMapModel(const char* data, uint32_t size, std::string dir) {
    
}


/*=====================================================> PARSERS <=================================================================*/

bool isPointInPolygon(glm::vec2 point, std::vector<glm::vec2> polygon, bool onEdges = false) {
    bool is_in = 0;
    
    for (uint32_t i = 0; i < polygon.size(); i++) {
        //std::cout << i << " - " << polygon[i].x << " " << polygon[i].y << "\n";
        glm::vec2 a = polygon[i];
        glm::vec2 b;
        if (i == polygon.size()-1)
            b = polygon[0];
        else
            b = polygon[i+1];        
        
        float xByY = 0;
        if (std::abs(a.y - b.y) < 0.01f) continue;
        if (std::abs(a.x - b.x) < 0.01f)
            xByY = a.x;
        else
            xByY = a.x + (b.x - a.x) * (point.y - a.y) / (b.y - a.y);

        if (std::abs(point.x - xByY) < 0.01f) {
            //std::cout << xByY << "\n";
            return false;
        }

        float min_y = std::min(a.y, b.y);
        float max_y = std::max(a.y, b.y);

        if (point.y > min_y && point.y < max_y && point.x < (b.x - a.x) * (point.y - a.y) / (b.y - a.y) + a.x) {
            is_in = !is_in;
        }
    }

    return is_in;
}

struct ObjObject {
    std::string name;
    std::vector<std::vector<int32_t>> indices;
};

void ObjMapModel::parseObj(std::string data) {
    std::vector<glm::vec3> vertices;
    std::vector<ObjObject> objects;

    ObjObject* current_obj;

    std::string line;
    std::istringstream tokenStream(data);
    while (std::getline(tokenStream, line, '\n')) {
        std::vector<std::string> words = split(line, ' ');

        if (words.size() == 0) continue;

        if (words[0] == "v") {
            if (words.size() < 4) continue;
            vertices.push_back(glm::vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3])));
        }
        else if (words[0] == "f") {
            if (words.size() < 4) continue;

            std::vector <int32_t> v_indices;
            for (uint32_t i = 1; i < words.size(); i++) {
                std::vector<std::string> indices = split(words[i], '/');

                // TODO: ONLY VERTICES
                int32_t v_i = std::stoi(indices[0]);
                v_indices.push_back(v_i - (v_i > 0 ? 1 : 0));
            }

            current_obj->indices.push_back(v_indices);
        }
        else if (words[0] == "vt") {
        }
        else if (words[0] == "vn") {
        }
        else if (words[0] == "usemtl") {
        }
        else if (words[0] == "mtllib") {
        }
        else if (words[0] == "o") {
            if (words.size() < 2) continue;
            objects.push_back(ObjObject());
            current_obj = &objects[objects.size() - 1];
            current_obj->name = words[1];
            current_obj->indices.clear();
        }
    }

    std::cout << "Size: " << vertices.size() << "\n";

    for (auto obj : objects) {
        std::cout << "Object: " << obj.name << "\n";
        for (auto i : obj.indices) {
            for (auto j : i) {
                std::cout << j << " ";
            }
            std::cout << "\n";
        }
    }


    for (auto obj : objects) {
        std::vector<int32_t> all_indices;
        std::vector<float> mesh_vertices;
        std::vector <uint32_t> mesh_indicies;
        //Mesh mesh;

        for (auto i : obj.indices) {

            glm::vec3 v0 = vertices[i[0]];
            glm::vec3 v1 = vertices[i[1]];
            glm::vec3 v2 = vertices[i[2]];
            glm::vec3 v3 = vertices[i[3]];

            glm::vec3 AB = v1 - v0;
            glm::vec3 AC = v2 - v0;

            glm::vec3 N = glm::cross(AB, AC);

            glm::vec3 U = glm::normalize(AB);
            glm::vec3 uN = glm::normalize(N);
            glm::vec3 V = glm::cross(U, uN);

            //std::cout << "normal: " << uN.x << " | " << uN.y << " | " << uN.z << "\n";

            // Матрица афинного преобразования
            glm::mat3 T = glm::mat3(
                U.x, V.x, N.x,
                U.y, V.y, N.y,
                U.z, V.z, N.z
            );

            glm::vec3 v0_z_null = T * v0;
            //v0_z_null.z = 0.0f;

            //std::cout << "v0_z_null: " << v0_z_null.x << " | " << v0_z_null.y << " | " << v0_z_null.z << "\n";

            glm::vec3 v1_z_null = T * v1;            
            //std::cout << "v1_z_null: " << v1_z_null.x << " | " << v1_z_null.y << " | " << v1_z_null.z << "\n";

            glm::vec3 v2_z_null = T * v2;
            //std::cout << "v2_z_null: " << v2_z_null.x << " | " << v2_z_null.y << " | " << v2_z_null.z << "\n";

            glm::vec3 v3_z_null = T * v3;
            //std::cout << "v3_z_null: " << v3_z_null.x << " | " << v3_z_null.y << " | " << v3_z_null.z << "\n";

            glm::mat3 T_inv = glm::inverse(T);

            glm::vec3 v0_z_not_null = T_inv * v0_z_null;

            //std::cout << "v0_z_not_null: " << v0_z_not_null.x << " | " << v0_z_not_null.y << " | " << v0_z_not_null.z << "\n";

            //std::cout << "v0: " << v0.x << " | " << v0.y << " | " << v0.z << "\n";


            //std::cout << "\n";
            std::vector<glm::vec2> poly = {
                {v0_z_null.x, v0_z_null.y},
                {v1_z_null.x, v1_z_null.y},
                {v2_z_null.x, v2_z_null.y},
                {v3_z_null.x, v3_z_null.y}
            };

            for (auto p : poly) {
                //std::cout << p.x << " " << p.y << "\n";
            }
            
            glm::vec2 poly_min = poly[0];
            glm::vec2 poly_max = poly[0];
            for (auto p : poly) {
                if (p.x < poly_min.x) poly_min.x = p.x;
                if (p.y < poly_min.y) poly_min.y = p.y;
                if (p.x > poly_max.x) poly_max.x = p.x;
                if (p.y > poly_max.y) poly_max.y = p.y;
            }

            float count = 0;
            float step = 1;
            
            //const uint32_t x_iter_count = (uint32_t)std::floor((poly_max.x - poly_min.x) / step) + 1;
            //std::cout << "x_iter_count: " << x_iter_count << "\n";
            std::vector<glm::vec2> grid;
            for (float x = poly_min.x - step; x < poly_max.x;) {
                if (std::abs(poly_max.x - x) < step)
                    x = poly_max.x;
                else 
                    x += step;

                for (float y = poly_min.y - step; y < poly_max.y;) {
                    if (std::abs(poly_max.y - y) < step)
                        y = poly_max.y;
                    else 
                        y += step;
                    
                    //std::cout << x << " " << y << "\n";
                    if (isPointInPolygon({x, y}, poly)) {

                        //count++;
                        grid.push_back({x, y});
                        //std::cout << "[" << x << ", " << y << "],\n";
                    }
                }
            }

            //for ()

            //poly_max.x += 0.01;
            for (float x = poly_min.x; x < poly_max.x; x += step) {
                for (float y = poly_min.y; y < poly_max.y; y += step) {
                    glm::vec2 a = {x, y};
                    glm::vec2 b = {x + step, y + step};
                    glm::vec2 points[4] = {
                        {x, y},
                        {x + step, y},
                        {x + step, y + step},
                        {x, y + step}
                    };

                    std::vector<uint32_t> cell_grid_indices;
                    for (uint32_t i = 0; i < grid.size(); i++) {
                        glm::vec2 &p = grid[i];
                        if (a.x <= p.x && p.x <= b.x && a.y <= p.y && p.y <= b.y) {
                            cell_grid_indices.push_back(i);
                        }
                    }

                    /*for (const auto& p : poly) {
                        //(b.x - a.x) * (point.y - a.y) / (b.y - a.y) + a.x
                        auto xByY_f = [](float y) -> float {
                            return 
                        }
                    }*/

                

                    std::cout << "[" << x << ", " << y << "] - {";
                    for (uint32_t i = 0; i < cell_grid_indices.size(); i++) {
                        std::cout << "[" << grid[cell_grid_indices[i]].x << ", " << grid[cell_grid_indices[i]].y << "], ";

                        
                    }
                    std::cout << "}\n";
                }
            }


            //for (uint32_t i = 0; i < ; i++) {

            std::cout << "count: " << count << "\n";
            //exit(0);
            //std::cout << a << " " << b / a << "\n";


            //vertices[i[0]] = v0_z_not_null;

            mesh_indicies.push_back(mesh_vertices.size() / 3);
            mesh_indicies.push_back(mesh_vertices.size() / 3 + 1);
            mesh_indicies.push_back(mesh_vertices.size() / 3 + 2);
            mesh_indicies.push_back(mesh_vertices.size() / 3 + 3);
            mesh_indicies.push_back(mesh_vertices.size() / 3 + 4);
            mesh_indicies.push_back(mesh_vertices.size() / 3 + 5);

            glm::vec3 vert = v0_z_not_null;//vertices[i[0]];
            mesh_vertices.push_back(vert.x);
            mesh_vertices.push_back(vert.y);
            mesh_vertices.push_back(vert.z);

            vert = vertices[i[1]];
            mesh_vertices.push_back(vert.x);
            mesh_vertices.push_back(vert.y);
            mesh_vertices.push_back(vert.z);

            vert = vertices[i[2]];
            mesh_vertices.push_back(vert.x);
            mesh_vertices.push_back(vert.y);
            mesh_vertices.push_back(vert.z);

            mesh_vertices.push_back(vert.x);
            mesh_vertices.push_back(vert.y);
            mesh_vertices.push_back(vert.z);

            vert = vertices[i[3]];
            mesh_vertices.push_back(vert.x);
            mesh_vertices.push_back(vert.y);
            mesh_vertices.push_back(vert.z);

            vert = v0_z_not_null;//vertices[i[0]];
            mesh_vertices.push_back(vert.x);
            mesh_vertices.push_back(vert.y);
            mesh_vertices.push_back(vert.z);
        }
        

        Mesh* mesh = new Mesh(mesh_vertices, mesh_indicies, mesh_vertices);
        this->meshes.push_back(mesh);
    }

}


/*==========================================================> SRING FUNCS <==============================================================*/

std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> result;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}