#include <HateEngine/Resources/ObjMapModel.hpp>
#include <HateEngine/Resources/Texture.hpp>
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "HateEngine/Log.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/matrix.hpp"

using namespace HateEngine;

std::vector<std::string> split(std::string str, char delimiter);

ObjMapModel::ObjMapModel(std::string obj_filename, std::string map_file_name) {
    std::ifstream file(obj_filename);

    std::string data((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    // std::cout << data << "\n";
    auto t0 = std::chrono::high_resolution_clock::now();
    parseObj(data);
    auto t1 = std::chrono::high_resolution_clock::now();
    std::cout << "Parsing took: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << " ms\n";
}

ObjMapModel::ObjMapModel(const char* data, uint32_t size, std::string dir) {
}

/*======================================> PARSERS <==============================================*/

inline static bool isPointInPolygon(glm::vec2 point, std::vector<glm::vec2> polygon) {
    bool is_in = 0;

    for (uint32_t i = 0; i < polygon.size(); i++) {
        // std::cout << i << " - " << polygon[i].x << " " << polygon[i].y <<
        // "\n";
        glm::vec2 a = polygon[i];
        glm::vec2 b;
        if (i == polygon.size() - 1)
            b = polygon[0];
        else
            b = polygon[i + 1];

        float xByY = 0;
        if (std::abs(a.y - b.y) < 0.01f)
            continue;
        if (std::abs(a.x - b.x) < 0.01f)
            xByY = a.x;
        else
            xByY = a.x + (b.x - a.x) * (point.y - a.y) / (b.y - a.y);

        if (std::abs(point.x - xByY) < 0.01f) {
            // std::cout << xByY << "\n";
            return false;
        }

        float min_y = std::min(a.y, b.y);
        float max_y = std::max(a.y, b.y);

        if (point.y > min_y && point.y < max_y && point.x < xByY) {
            is_in = !is_in;
        }
    }

    return is_in;
}

struct ObjFace {
    std::vector<int32_t> indices;
    float normal[3] = {0.0f, 0.0f, 0.0f};
    std::vector<int32_t> tex_indices;
};

struct ObjObject {
    std::string name;
    std::vector<ObjFace> faces;
};

void ObjMapModel::parseObj(std::string data) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> tex_coords;
    std::vector<ObjObject> objects;

    ObjObject* current_obj;

    std::string line;
    std::istringstream tokenStream(data);
    while (std::getline(tokenStream, line, '\n')) {
        std::vector<std::string> words = split(line, ' ');

        if (words.size() == 0)
            continue;

        if (words[0] == "v") {
            if (words.size() < 4)
                continue;
            vertices.push_back(
                    glm::vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]))
            );
        } else if (words[0] == "f") {
            if (words.size() < 4)
                continue;

            std::vector<int32_t> v_indices;
            std::vector<int32_t> n_indices;
            std::vector<int32_t> t_indices;
            int32_t prev_n_index = 0;
            for (uint32_t i = 1; i < words.size(); i++) {
                std::vector<std::string> indices = split(words[i], '/');
                // HATE_DEBUG_F("Indices: %s", words[i].c_str())
                // HATE_DEBUG_F("Line: %s", line.c_str())

                int32_t v_i = std::stoi(indices[0]);
                v_indices.push_back(v_i - (v_i > 0 ? 1 : 0));

                if (indices.size() >= 2 and indices[1] != "") {
                    // Tex coords
                    int32_t t_i = std::stoi(indices[1]);
                    t_indices.push_back(t_i - (t_i > 0 ? 1 : 0));
                }

                if (indices.size() >= 3) {
                    // Normals
                    int32_t n_i = std::stoi(indices[2]);
                    n_indices.push_back(n_i - (n_i > 0 ? 1 : 0));

                    if (i > 1) {
                        if (prev_n_index != n_i) {
                            std::cout << "Different normals\n";
                        }
                    }
                    prev_n_index = n_i;
                }
            }

            // current_obj->indices.push_back(v_indices);
            // current_obj->normals.push_back({0.0f, 0.0f, 0.0f});
            // current_obj->faces.push_back({v_indices, {0.0f, 0.0f, 0.0f}});
            ObjFace face;
            face.indices = v_indices;
            face.tex_indices = t_indices;
            face.normal[0] = normals[n_indices[0]].x;
            face.normal[1] = normals[n_indices[0]].y;
            face.normal[2] = normals[n_indices[0]].z;

            current_obj->faces.push_back(face);
        } else if (words[0] == "vt") {
            if (words.size() < 3)
                continue;
            tex_coords.push_back(glm::vec2(std::stof(words[1]), std::stof(words[2])));
        } else if (words[0] == "vn") {
            if (words.size() < 4)
                continue;
            normals.push_back(
                    glm::vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]))
            );
        } else if (words[0] == "usemtl") {
        } else if (words[0] == "mtllib") {
        } else if (words[0] == "o") {
            if (words.size() < 2)
                continue;
            objects.push_back(ObjObject());
            current_obj = &objects[objects.size() - 1];
            current_obj->name = words[1];
            current_obj->faces.clear();
        }
    }

    std::cout << "Size: " << vertices.size() << "\n";

    /*for (auto obj : objects) {
        std::cout << "Object: " << obj.name << "\n";
        for (auto i : obj.indices) {
            for (auto j : i) {
                std::cout << j << " ";
            }
            std::cout << "\n";
        }
    }*/

    uint64_t c = 0;
    for (auto obj: objects) {
        std::vector<int32_t> all_indices;
        std::vector<float> mesh_vertices;
        std::vector<uint32_t> mesh_indicies;
        std::vector<float> mesh_normals;
        std::vector<float> mesh_UVs;
        // Mesh mesh;

        std::vector<ObjFace> of = {obj.faces[0]};
        for (const auto& face: obj.faces) {
            // for (const auto& face: of) {
            if (face.indices.size() < 3) {
                continue;
            }

            glm::vec3 v0 = vertices[face.indices[0]];
            glm::vec3 v1 = vertices[face.indices[1]];
            glm::vec3 v2 = vertices[face.indices[2]];
            // glm::vec3 v3 = vertices[face.indices[3]];

            glm::vec3 AB = v1 - v0;
            glm::vec3 AC = v2 - v0;

            glm::vec3 N = glm::cross(AB, AC);

            glm::vec3 U = glm::normalize(AB);
            glm::vec3 uN = glm::normalize(N);
            glm::vec3 V = glm::cross(U, uN);

            // Матрица афинного преобразования
            glm::mat3 T = glm::mat3(U.x, V.x, N.x, U.y, V.y, N.y, U.z, V.z, N.z);
            glm::mat3 T_inv = glm::inverse(T);

            std::vector<glm::vec2> poly;
            for (const auto& index: face.indices) {
                glm::vec3 v = vertices[index];
                glm::vec3 v_z_null = T * v;
                poly.push_back(glm::vec2(v_z_null.x, v_z_null.y));
            }

            glm::vec3 v0_z_null = T * v0;
            const float KEY = v0_z_null.z;

            glm::vec2 poly_min = poly[0];
            glm::vec2 poly_max = poly[0];
            uint32_t min_index_x = 0;
            uint32_t max_index_x = 0;
            uint32_t min_index_y = 0;
            uint32_t max_index_y = 0;
            for (uint32_t i = 1; i < poly.size(); i++) {
                glm::vec2 p = poly[i];
                if (p.x < poly_min.x) {
                    poly_min.x = p.x;
                    min_index_x = i;
                }
                if (p.y < poly_min.y) {
                    poly_min.y = p.y;
                    min_index_y = i;
                }
                if (p.x > poly_max.x) {
                    poly_max.x = p.x;
                    max_index_x = i;
                }
                if (p.y > poly_max.y) {
                    poly_max.y = p.y;
                    max_index_y = i;
                }
            }

            glm::vec2 start_tex = {
                    tex_coords[face.tex_indices[min_index_x]].x,
                    tex_coords[face.tex_indices[min_index_y]].y
            };
            glm::vec2 end_tex = {
                    tex_coords[face.tex_indices[max_index_x]].x,
                    tex_coords[face.tex_indices[max_index_y]].y
            };

            const glm::vec2 UV_Vertex_K = {
                    (end_tex.x - start_tex.x) / (poly_max.x - poly_min.x),
                    (end_tex.y - start_tex.y) / (poly_max.y - poly_min.y)
            };

            HATE_DEBUG_F("%d | %d", poly.size(), face.tex_indices.size())
            // HATE_DEBUG_F("x: %f, y: %f", UV_Vertex_K.x, UV_Vertex_K.y);

            // HATE_DEBUG_F("[%f, %f] - [%f, %f]", start_tex.x, end_tex.x, start_tex.y, end_tex.y);
            // HATE_DEBUG_F("{%f, %f} - {%f, %f}", poly_min.x, poly_max.x, poly_min.y, poly_max.y);

            float step = 500.0f;

            std::vector<glm::vec2> grid;
            grid.reserve(
                    (poly_max.x - poly_min.x) * (poly_max.y - poly_min.y) / (step * step) +
                    poly.size()
            );
            // Add to grid points inside the polygon
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

                    if (isPointInPolygon({x, y}, poly)) {
                        grid.push_back({x, y});
                    }
                }
            }

            // Add to grid points on the edges of the polygon
            for (uint32_t i = 0; i < poly.size(); i++) {
                glm::vec2 a = poly[i];
                glm::vec2 b;
                if (i == poly.size() - 1)
                    b = poly[0];
                else
                    b = poly[i + 1];

                float min_y = std::min(a.y, b.y);
                float max_y = std::max(a.y, b.y);
                float min_x = std::min(a.x, b.x);
                float max_x = std::max(a.x, b.x);
                for (float x = poly_min.x + step; x < max_x; x += step) {
                    if (x <= min_x)
                        continue;
                    float yByX = a.y + (b.y - a.y) * (x - a.x) / (b.x - a.x);
                    grid.push_back({x, yByX});
                }

                for (float y = poly_min.y + step; y < max_y; y += step) {
                    if (y <= min_y)
                        continue;
                    float xByY = a.x + (b.x - a.x) * (y - a.y) / (b.y - a.y);
                    grid.push_back({xByY, y});
                }
            }

            // Add to grid polygon vertices
            for (const auto p: poly) {
                grid.push_back(p);
            }

            /*uint32_t g_x = 0;
            for (float x = poly_min.x; x < poly_max.x; x += step) {g_x++;}
            uint32_t g_y = 0;
            for (float y = poly_min.y; y < poly_max.y; y += step) {g_y++;}*/

            uint32_t g_x = std::ceil((poly_max.x - poly_min.x) / step);
            uint32_t g_y = std::ceil((poly_max.y - poly_min.y) / step);

            std::vector<std::vector<std::vector<uint32_t>>> cell_grid;
            cell_grid.reserve(g_x);
            for (uint32_t i = 0; i < g_x; i++) {
                cell_grid.push_back(std::vector<std::vector<uint32_t>>());
                cell_grid[i].reserve(g_y);
                for (uint32_t j = 0; j < g_y; j++) {
                    cell_grid[i].push_back(std::vector<uint32_t>());
                }
            }

            for (uint32_t i = 0; i < grid.size(); ++i) {
                uint32_t x = (uint32_t) std::floor((grid[i].x - poly_min.x) / step);
                uint32_t y = (uint32_t) std::floor((grid[i].y - poly_min.y) / step);

                uint32_t start_x = x > 0 ? x - 1 : 0;
                uint32_t start_y = y > 0 ? y - 1 : 0;
                uint32_t end_x = x < g_x - 1 ? x + 1 : g_x - 1;
                uint32_t end_y = y < g_y - 1 ? y + 1 : g_y - 1;

                for (uint32_t x = start_x; x <= end_x; x++) {
                    for (uint32_t y = start_y; y <= end_y; y++) {
                        glm::vec2 a = {poly_min.x + x * step - 0.01, poly_min.y + y * step - 0.01};
                        glm::vec2 b = {
                                poly_min.x + (x + 1) * step + 0.01,
                                poly_min.y + (y + 1) * step + 0.01
                        };
                        glm::vec2 p = grid[i];

                        if (((a.x < p.x && p.x < b.x)) && ((a.y < p.y && p.y < b.y))) {
                            cell_grid[x][y].push_back(i);
                        }
                    }
                }
            }

            for (uint32_t i = 0; i < cell_grid.size(); i++) {
                for (uint32_t j = 0; j < cell_grid[i].size(); j++) {
                    std::vector<uint32_t> cell = cell_grid[i][j];

                    if (cell.size() < 3) {
                        continue;
                    }

                    float min_x = 0;
                    float max_x = 0;
                    float min_y = 0;
                    float max_y = 0;
                    for (uint32_t i = 0; i < cell.size(); i++) {
                        if (i == 0) {
                            min_x = grid[cell[i]].x;
                            max_x = grid[cell[i]].x;
                            min_y = grid[cell[i]].y;
                            max_y = grid[cell[i]].y;
                        } else {
                            min_x = std::min(min_x, grid[cell[i]].x);
                            max_x = std::max(max_x, grid[cell[i]].x);
                            min_y = std::min(min_y, grid[cell[i]].y);
                            max_y = std::max(max_y, grid[cell[i]].y);
                        }
                    }

                    float center_x = (min_x + max_x) / 2;
                    float center_y = (min_y + max_y) / 2;

                    // sort in counter clockwise order
                    std::sort(
                            cell.begin(), cell.end(),
                            [&center_x, &center_y, &grid](uint32_t a, uint32_t b) -> bool {
                                glm::vec2 a_v = grid[a];
                                glm::vec2 b_v = grid[b];
                                float a_angle = std::atan2(a_v.y - center_y, a_v.x - center_x);
                                float b_angle = std::atan2(b_v.y - center_y, b_v.x - center_x);
                                return (a_angle > b_angle);
                            }
                    );

                    for (auto c: cell) {
                        auto p = grid[c];
                        // HATE_DEBUG_F("Cx: %f, Cy: %f", p.x, p.y);
                    }
                    // std::cout << std::endl;

                    // cell.push_back(cell[0]);
                    // HATE_DEBUG_F("Cell size: %d", cell.size());
                    for (uint32_t i = 2; i < cell.size(); ++i) {
                        mesh_indicies.push_back(mesh_vertices.size() / 3 + cell[0]);
                        mesh_indicies.push_back(mesh_vertices.size() / 3 + cell[i - 1]);
                        mesh_indicies.push_back(mesh_vertices.size() / 3 + cell[i]);
                    }

                    /*for (const auto c : cell) {
                        glm::vec2 p = grid[c] * UV_Vertex_K;
                        mesh_UVs.push_back(p.x);
                        mesh_UVs.push_back(p.y);
                    }*/

                    /*glm::vec2 p = grid[cell[0]] * UV_Vertex_K;

                    mesh_UVs.push_back(p.x);
                    mesh_UVs.push_back(p.y);
                    HATE_DEBUG_F("x: %f | y: %f", p.x, p.y);
                    p = grid[cell[1]] * UV_Vertex_K;
                    mesh_UVs.push_back(p.x);
                    mesh_UVs.push_back(p.y);
                    HATE_DEBUG_F("x: %f | y: %f", p.x, p.y);

                    p = grid[cell[2]] * UV_Vertex_K;
                    mesh_UVs.push_back(p.x);
                    mesh_UVs.push_back(p.y);
                    HATE_DEBUG_F("x: %f | y: %f", p.x, p.y);
                    std::cout << std::endl;

                    p = grid[cell[3]] * UV_Vertex_K;
                    mesh_UVs.push_back(p.x);
                    mesh_UVs.push_back(p.y);
                    HATE_DEBUG_F("x: %f | y: %f", p.x, p.y);
                    std::cout << std::endl;*/
                }
            }

            for (const auto& p: grid) {
                glm::vec3 a = {p.x, p.y, KEY};
                glm::vec3 new_a = T_inv * a;
                mesh_vertices.push_back(new_a.x);
                mesh_vertices.push_back(new_a.y);
                mesh_vertices.push_back(new_a.z);

                mesh_normals.push_back(face.normal[0]);
                mesh_normals.push_back(face.normal[1]);
                mesh_normals.push_back(face.normal[2]);

                glm::vec2 uv = p * UV_Vertex_K;
                mesh_UVs.push_back(uv.x);
                mesh_UVs.push_back(uv.y);
            }
        }

        // HATE_DEBUG_F("Indi. size: %lu | UV size: %lu | %d", mesh_indicies.size(),
        // mesh_UVs.size(), mesh_UVs.size() == mesh_indicies.size()*2)
        //

        // Calculate mesh offset
        glm::vec3 min_coords = {
                std::numeric_limits<float>::max(), std::numeric_limits<float>::max(),
                std::numeric_limits<float>::max()
        };
        glm::vec3 max_coords = {
                std::numeric_limits<float>::min(), std::numeric_limits<float>::min(),
                std::numeric_limits<float>::min()
        };
        for (uint32_t i = 0; i < mesh_vertices.size(); i += 3) {
            min_coords.x = std::min(min_coords.x, mesh_vertices[i]);
            min_coords.y = std::min(min_coords.y, mesh_vertices[i + 1]);
            min_coords.z = std::min(min_coords.z, mesh_vertices[i + 2]);

            max_coords.x = std::max(max_coords.x, mesh_vertices[i]);
            max_coords.y = std::max(max_coords.y, mesh_vertices[i + 1]);
            max_coords.z = std::max(max_coords.z, mesh_vertices[i + 2]);
        }

        glm::vec3 center = (min_coords + max_coords) / 2.0f;
        for (uint32_t i = 0; i < mesh_vertices.size(); i += 3) {
            mesh_vertices[i] -= center.x;
            mesh_vertices[i + 1] -= center.y;
            mesh_vertices[i + 2] -= center.z;
        }


        Mesh* mesh = new Mesh(mesh_vertices, mesh_indicies, mesh_normals);
        mesh->setPosition(center);
        mesh->setUV(mesh_UVs);
        Texture* texture = new Texture("examples/Assets/brick.png");
        mesh->setTexture(texture);
        // mesh->disableLightShading();

        this->meshes.push_back(mesh);
    }
}

/*==========================================================> SRING FUNCS
 * <==============================================================*/

std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> result;
    std::string token;
    std::istringstream tokenStream(str);
    while (std::getline(tokenStream, token, delimiter)) {
        // HATE_DEBUG_F("Token: '%s'", token.c_str())
        if (not token.empty())
            result.push_back(token);
    }
    return result;
}
