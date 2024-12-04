#include <HateEngine/Resources/ObjMapModel.hpp>
#include <HateEngine/Resources/Texture.hpp>
#include <HateEngine/Resources/HERFile.hpp>
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "HateEngine/Log.hpp"
#include "HateEngine/Objects/Physics/ConvexShape.hpp"
#include "HateEngine/Objects/Physics/StaticBody.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/matrix.hpp"

using namespace HateEngine;


std::vector<std::string> split(const std::string& str, const std::string& delimiters);
void trim(std::string& str);


ObjMapModel::ObjMapModel(
        std::string obj_filename, std::string map_file_name, float grid_size,
        bool generate_collision, float lod_dist, float lod_step
) {
    bindObj(&static_body);
    this->generate_collision = generate_collision;
    this->obj_file_name = obj_filename;
    this->map_file_name = map_file_name;
    // get parrent from obj_filename
    size_t pos = obj_filename.find_last_of("/\\");
    if (pos != std::string::npos)
        obj_file_path = obj_filename.substr(0, pos + 1);

    std::ifstream file(obj_filename);
    if (!file.is_open()) {
        HATE_ERROR_F("Failed to open file: %s", obj_filename.c_str());
        return;
    }

    std::string data((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    auto t0 = std::chrono::high_resolution_clock::now();
    parseObj(data, grid_size, lod_dist, lod_step);
    auto t1 = std::chrono::high_resolution_clock::now();
    HATE_DEBUG_F(
            "Parsing took: %d ms",
            std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()
    );

    std::ifstream map_file(map_file_name);
    if (!map_file.is_open()) {
        HATE_ERROR_F("Failed to open file: %s", map_file_name.c_str());
        return;
    }
    std::string map_data(
            (std::istreambuf_iterator<char>(map_file)), (std::istreambuf_iterator<char>())
    );
    parseMap(map_data, grid_size);
}

ObjMapModel::ObjMapModel(
        std::string obj_file_data, std::string map_file_data, HERFile* her, float grid_size,
        bool generate_collision, float lod_dist, float lod_step

) {
    bindObj(&static_body);
    this->generate_collision = generate_collision;
    parseObj(obj_file_data, grid_size, lod_dist, lod_step, her);
    parseMap(map_file_data, grid_size);
}

/*======================================> PARSERS <==============================================*/
void ObjMapModel::parseObj(
        std::string data, float grid_size, float lod_dist, float lod_step, HERFile* her
) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> tex_coords;
    std::vector<ObjObject> objects;

    ObjObject* current_obj;

    std::string line;
    std::istringstream tokenStream(data);
    while (std::getline(tokenStream, line, '\n')) {
        if (line[line.size() - 1] == 13) // CR
            line = line.substr(0, line.size() - 1);
        std::vector<std::string> words = split(line, " \v\t");

        if (words.size() == 0)
            continue;

        if (words[0] == "v") {
            if (words.size() < 4)
                continue;
            vertices.push_back(glm::vec3(
                    std::stof(words[1]) / grid_size, std::stof(words[2]) / grid_size,
                    std::stof(words[3]) / grid_size
            ));
        } else if (words[0] == "f") {
            if (words.size() < 4)
                continue;

            std::vector<int32_t> v_indices;
            std::vector<int32_t> n_indices;
            std::vector<int32_t> t_indices;
            int32_t prev_n_index = 0;
            for (uint32_t i = 1; i < words.size(); i++) {
                std::vector<std::string> indices = split(words[i], "/");
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
                            HATE_WARNING("Different normals");
                        }
                    }
                    prev_n_index = n_i;
                }
            }

            /*std::string ti;
            for (uint32_t i = 0; i < t_indices.size(); i++) {
                ti += std::to_string(t_indices[i]);
                if (i < t_indices.size() - 1)
                    ti += " ";
            }
            HATE_WARNING_F("Indices: %s", ti.c_str());*/

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
            // HATE_WARNING_F("Tex coords: %f %f", std::stof(words[1]), std::stof(words[2]));
        } else if (words[0] == "vn") {
            if (words.size() < 4)
                continue;
            normals.push_back(
                    glm::vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3]))
            );
        } else if (words[0] == "usemtl") {
            if (words.size() < 2 || current_obj == nullptr)
                continue;
            if (current_obj->material == "")
                current_obj->material = words[1];
        } else if (words[0] == "mtllib") {
            if (words.size() < 2)
                continue;

            // Load from HateEngine Resource File
            if (her != nullptr) {
                std::string mtl = (*her)[words[1]].asString();
                materials = parseMtlLib(mtl, her);
            } else {
                std::ifstream file(obj_file_path + words[1]);
                if (!file.is_open()) {
                    HATE_ERROR_F("Could not open file: %s", (obj_file_path + words[1]).c_str());
                    continue;
                }
                std::string mtl(
                        (std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>())
                );
                materials = parseMtlLib(mtl, nullptr);
            }

        } else if (words[0] == "o") {
            if (words.size() < 2)
                continue;
            objects.push_back(ObjObject());
            current_obj = &objects[objects.size() - 1];
            current_obj->name = words[1];
            current_obj->faces.clear();
        }
    }

    addLOD(0, generateLod(vertices, tex_coords, objects, lod_step));
    addLOD(lod_dist, generateLod(vertices, tex_coords, objects, 1000000));

    if (generate_collision)
        this->generateCollision(vertices, objects);
}


std::unordered_map<std::string, ObjMapModel::Material> ObjMapModel::parseMtlLib(
        std::string data, HERFile* her
) {
    std::unordered_map<std::string, Material> materials;

    Material* current_material = nullptr;

    std::string line;
    std::istringstream tokenStream(data);
    while (std::getline(tokenStream, line, '\n')) {
        if (line[line.size() - 1] == 13) // CR
            line = line.substr(0, line.size() - 1);
        std::vector<std::string> words = split(line, " \v\t");

        if (words.size() == 0)
            continue;

        if (words[0] == "newmtl") {
            if (words.size() < 2)
                continue;
            materials[words[1]] = Material();
        }

        if (words[0] == "map_Kd") {
            if (words.size() < 2)
                continue;

            if (her != nullptr) {
                this->textures.push_back((*her)[words[1]].asTexture());
            } else {
                this->textures.push_back(Texture(obj_file_path + words[1]));
            }
            materials[words[0]].texture_id = this->textures.size() - 1;
        }
    }

    return materials;
}


struct Entity {
    std::string classname;
    glm::vec3 pos;
    std::unordered_map<std::string, std::string> properties;
};

void ObjMapModel::parseMap(std::string data, float grid_size) {
    std::vector<Entity> entities;
    Entity* current_entity = nullptr;

    std::string line;
    std::istringstream tokenStream(data);
    uint32_t line_num = 0;
    uint32_t bracket_depth = 0; // 1 = entity, 2 = brush
    while (std::getline(tokenStream, line, '\n')) {
        if (line[line.size() - 1] == 13) // CR
            line = line.substr(0, line.size() - 1);
        // std::vector<std::string> words = split(line, " \v\t");
        std::vector<std::string> words;
        std::string word;
        bool in_quotes = false;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == '"') {
                in_quotes = !in_quotes;
                if (not in_quotes and not word.empty()) {
                    words.push_back(word);
                    word.clear();
                }
            } else if (line[i] == '\\' and i < line.size() - 1) {
                if (line[i + 1] == '"') {
                    word += '"';
                    i++;
                } else if (line[i + 1] == '\\') {
                    word += '\\';
                    i++;
                }
            } else
                word += line[i];
        }
        line_num++;

        if (line[0] == '/' and line[1] == '/')
            continue;

        // Skip brushes
        if (bracket_depth == 2) {
            if (line[0] == '}')
                bracket_depth--;

            continue;
        }


        if (line[0] == '{') {
            bracket_depth++;
            if (bracket_depth > 2) {
                HATE_ERROR_F(
                        "ObjMapModel [%s]: Unexpected entity depth (%u) at line %d",
                        map_file_name.c_str(), bracket_depth, line_num
                );
                return;
            }

            if (bracket_depth == 1) {
                HATE_DEBUG("");
                entities.push_back(Entity());
                current_entity = &entities[entities.size() - 1];
            }
        } else if (line[0] == '}') {
            if (bracket_depth == 0) {
                HATE_ERROR_F(
                        "ObjMapModel [%s]: Unexpected entity depth (%u) at line %d",
                        map_file_name.c_str(), bracket_depth, line_num
                );
                return;
            }
            bracket_depth--;
            current_entity = nullptr;
        } else {
            if (bracket_depth != 1) {
                HATE_WARNING_F(
                        "ObjMapModel [%s]: Unexpected entity depth (%u) for parametre at line %d, "
                        "skip",
                        map_file_name.c_str(), bracket_depth, line_num
                );
                continue;
            }

            if (words.size() < 2) {
                HATE_WARNING_F(
                        "ObjMapModel [%s]: Unexpected parametre at line %d, skip",
                        map_file_name.c_str(), line_num
                );

                for (uint32_t i = 0; i < words.size(); i++)
                    HATE_DEBUG_F("Word %d: '%s'", i, words[i].c_str());

                continue;
            }

            std::string value = words[1].substr(1, words[1].size() - 2);
            HATE_DEBUG_F(
                    "ObjMapModel [%s]: %s = %s", map_file_name.c_str(), words[0].c_str(),
                    value.c_str()
            );

            if (words[0] == "classname") {
                current_entity->classname = words[1];
            } else if (words[0] == "origin") {
                std::vector<std::string> coords = split(words[1], " ");
                // XZY
                current_entity->pos = glm::vec3(
                        std::stof(coords[0]) / grid_size, std::stof(coords[2]) / grid_size,
                        std::stof(coords[1]) / grid_size
                );
            } else {
                current_entity->properties[words[0]] = words[1];
            }
        }
    }
}


/*inline static bool isPointInPolygon(const glm::vec2 point, const std::vector<glm::vec2>& polygon)
{ bool is_in = 0;

    for (uint32_t i = 0; i < polygon.size(); i++) {
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
            return false;
        }

        float min_y = std::min(a.y, b.y);
        float max_y = std::max(a.y, b.y);

        if (point.y > min_y && point.y < max_y && point.x < xByY) {
            is_in = !is_in;
        }
    }

    return is_in;
}*/

// Вспомогательная функция для вычисления знака векторного произведения
inline float sign(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

// Проверка принадлежности точки выпуклой области
inline static bool isPointInPolygon(const glm::vec2& point, const std::vector<glm::vec2>& polygon) {
    bool has_neg = false;
    bool has_pos = false;

    for (size_t i = 0; i < polygon.size(); ++i) {
        const glm::vec2& v1 = polygon[i];
        const glm::vec2& v2 = polygon[(i + 1) % polygon.size()]; // Следующая вершина (циклически)

        float d = sign(point, v1, v2);
        if (d < 0)
            has_neg = true;
        if (d > 0)
            has_pos = true;

        // Если есть как отрицательные, так и положительные значения, точка вне области
        if (has_neg && has_pos)
            return false;
    }

    // Точка внутри или на границе
    return true;
}

// Основная функция для проверки, находится ли точка внутри треугольника
inline static bool isPointInTriangle(
        const glm::vec2 point, const std::vector<glm::vec2>& triangle
) {
    const glm::vec2& v1 = triangle[0];
    const glm::vec2& v2 = triangle[1];
    const glm::vec2& v3 = triangle[2];

    // Вычисляем знаки для векторных произведений
    float d1 = sign(point, v1, v2);
    float d2 = sign(point, v2, v3);
    float d3 = sign(point, v3, v1);

    // Проверяем, что все знаки одинаковые или равны нулю (на границе)
    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

inline static bool isPointOnTriangleEdge(
        const glm::vec2 p, const std::vector<glm::vec2>& triangle,
        std::array<glm::vec2, 2>* return_edge = nullptr
) {
    std::array<std::array<glm::vec2, 2>, 3> edges = {
            {{triangle[0], triangle[1]}, {triangle[1], triangle[2]}, {triangle[2], triangle[0]}}
    };

    for (uint32_t i = 0; i < 3; ++i) {
        glm::vec2 v1 = edges[i][0];
        glm::vec2 v2 = edges[i][1];
        glm::vec2 min = glm::min(v1, v2);
        glm::vec2 max = glm::max(v1, v2);

        if (std::abs(p.x - v1.x) < 0.01f and std::abs(v1.x - v2.x) < 0.01f) {
            if (p.y > min.y and p.y < max.y)
                return true;

        } else if (std::abs(p.y - v1.y) < 0.01f and std::abs(v1.y - v2.y) < 0.01f) {
            if (p.x > min.x and p.x < max.x)
                return true;

        } else if (p.y > min.y and p.y < max.y and p.x > min.x and p.x < max.x) {
            double m = (v2.y - v1.y) / (v2.x - v1.x);
            double b = (p.x - v1.x) * m + v1.y;
            if (std::abs(p.y - b) < 0.01f)
                return true;
        }
    }

    return false;
}

// Функция для вычисления барицентрических координат
glm::vec3 ComputeBarycentricCoordinates(
        const glm::vec2& p, const glm::vec2& v0, const glm::vec2& v1, const glm::vec2& v2
) {
    // Векторы
    glm::vec2 v0v1 = v1 - v0;
    glm::vec2 v0v2 = v2 - v0;
    glm::vec2 v0p = p - v0;

    // Детерминант (площадь треугольника)
    float denom = v0v1.x * v0v2.y - v0v1.y * v0v2.x;

    // Проверка на малый детерминант (почти вырожденный треугольник)
    if (fabs(denom) < 1e-6) {
        std::cout << "Degenerate triangle or too small area." << std::endl;
        return glm::vec3(-1.0f, -1.0f, -1.0f); // Возвращаем ошибочные координаты
    }

    // Барицентрические координаты
    float v = (v0p.x * v0v2.y - v0p.y * v0v2.x) / denom;
    float w = (v0v1.x * v0p.y - v0v1.y * v0p.x) / denom;
    float u = 1.0f - v - w;

    // Проверка на малые отклонения для числовых погрешностей (погрешности вычислений с плавающей
    // точкой)
    if (u < -1e-6 || v < -1e-6 || w < -1e-6 || u > 1.0f + 1e-6 || v > 1.0f + 1e-6 ||
        w > 1.0f + 1e-6) {
        /*std::cout << "Point outside the triangle (due to numerical issues or calculation errors)."
                  << std::endl;*/
        return glm::vec3(-1.0f, -1.0f, -1.0f); // Возвращаем ошибочные координаты
    }

    return glm::vec3(u, v, w);
}


std::vector<Mesh*> ObjMapModel::generateLod(
        std::vector<glm::vec3> vertices, std::vector<glm::vec2> tex_coords,
        std::vector<ObjObject> objects, float step
) {
    std::vector<Mesh*> meshes;
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

                if (std::abs(p.x - poly_min.x) < 0.01f) {
                    if (tex_coords[face.tex_indices[i]].x <
                        tex_coords[face.tex_indices[min_index_x]].x) {
                        min_index_x = i;
                    }
                }
                if (std::abs(p.y - poly_min.y) < 0.01f) {
                    if (tex_coords[face.tex_indices[i]].y <
                        tex_coords[face.tex_indices[min_index_y]].y) {
                        min_index_y = i;
                    }
                }
                if (std::abs(p.x - poly_max.x) < 0.01f) {
                    if (tex_coords[face.tex_indices[i]].x >
                        tex_coords[face.tex_indices[max_index_x]].x) {
                        max_index_x = i;
                    }
                }
                if (std::abs(p.y - poly_max.y) < 0.01f) {
                    if (tex_coords[face.tex_indices[i]].y >
                        tex_coords[face.tex_indices[max_index_y]].y) {
                        max_index_y = i;
                    }
                }
            }
            /*HATE_ERROR("POLY");
            for (uint32_t i = 0; i < poly.size(); i++) {
                HATE_WARNING_F("%f %f", poly[i].x, poly[i].y);
            }

            glm::vec2 start_tex = {
                    tex_coords[face.tex_indices[min_index_x]].x,
                    tex_coords[face.tex_indices[min_index_y]].y
            };
            glm::vec2 end_tex = {
                    tex_coords[face.tex_indices[max_index_x]].x,
                    tex_coords[face.tex_indices[max_index_y]].y
            };

            HATE_WARNING_F("start_tex: %f %f", start_tex.x, start_tex.y);
            HATE_WARNING_F("end_tex: %f %f", end_tex.x, end_tex.y);

            const glm::vec2 UV_Vertex_K = {
                    (end_tex.x - start_tex.x) / (poly_max.x - poly_min.x),
                    (end_tex.y - start_tex.y) / (poly_max.y - poly_min.y)
            };

            HATE_WARNING_F("UV_Vertex_K: %f %f", UV_Vertex_K.x, UV_Vertex_K.y);*/

            // Triangulate the polygon
            struct Triange {
                std::vector<glm::vec2> points;
                float invDenom;
                glm::vec2 v0; // v2 - v1
                glm::vec2 v1v3; // v3 - v1
                glm::vec2 tex[3];
            };

            std::vector<Triange> triangles;
            std::vector<uint32_t> poly_ccw_idx;
            glm::vec2 poly_center = {0, 0};
            poly_center.x = (poly_min.x + poly_max.x) / 2;
            poly_center.y = (poly_min.y + poly_max.y) / 2;

            for (uint32_t i = 0; i < poly.size(); i++) {
                poly_ccw_idx.push_back(i);
            }

            // sort in counter clockwise order

            std::sort(
                    poly_ccw_idx.begin(), poly_ccw_idx.end(),
                    [&poly_center, &poly](uint32_t& a, uint32_t& b) -> bool {
                        glm::vec2 a_v = poly[a];
                        glm::vec2 b_v = poly[b];
                        float a_angle = std::atan2(a_v.y - poly_center.y, a_v.x - poly_center.x);
                        float b_angle = std::atan2(b_v.y - poly_center.y, b_v.x - poly_center.x);
                        return (a_angle > b_angle);
                    }
            );

            for (uint32_t i = 2; i < poly_ccw_idx.size(); i++) {
                Triange t;
                t.points.push_back(poly[poly_ccw_idx[0]]);
                t.points.push_back(poly[poly_ccw_idx[i - 1]]);
                t.points.push_back(poly[poly_ccw_idx[i]]);
                /*t.tex_coords.push_back(tex_coords[face.tex_indices[0]]);
                t.tex_coords.push_back(tex_coords[face.tex_indices[i - 1]]);
                t.tex_coords.push_back(tex_coords[face.tex_indices[i]]);*/

                t.tex[0] = tex_coords[face.tex_indices[poly_ccw_idx[0]]];
                t.tex[1] = tex_coords[face.tex_indices[poly_ccw_idx[i - 1]]];
                t.tex[2] = tex_coords[face.tex_indices[poly_ccw_idx[i]]];

                glm::vec2 v1 = poly[poly_ccw_idx[0]];
                glm::vec2 v2 = poly[poly_ccw_idx[i - 1]];
                glm::vec2 v3 = poly[poly_ccw_idx[i]];

                // Вычисляем разности
                t.v0 = v2 - v1;
                t.v1v3 = v3 - v1;

                // Вычисляем обратное значение детерминанта
                float denom = t.v0.x * t.v1v3.y - t.v0.y * t.v1v3.x;
                t.invDenom = 1.0f / denom;

                if (std::abs(denom) < 1e-6f) {
                    HATE_ERROR("Degenerate triangle or numerical instability detected!");
                    continue;
                }

                /*t.tex_coords.push_back(tex_coords[face.tex_indices[0]]);
                t.tex_coords.push_back(tex_coords[face.tex_indices[i - 1]]);
                t.tex_coords.push_back(tex_coords[face.tex_indices[i]]);*/
                triangles.push_back(t);
            }

            /*for (uint32_t i = 0; i < triangles.size(); i++) {
                HATE_INFO("TRIANGLE");
                for (uint32_t j = 0; j < triangles[i].points.size(); j++) {
                    HATE_WARNING_F("%f %f", triangles[i].points[j].x, triangles[i].points[j].y);
                }
            }*/


            // triangulatePolygon(poly, triangles);

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

                    for (uint32_t i = 2; i < cell.size(); ++i) {
                        mesh_indicies.push_back(mesh_vertices.size() / 3 + cell[0]);
                        mesh_indicies.push_back(mesh_vertices.size() / 3 + cell[i - 1]);
                        mesh_indicies.push_back(mesh_vertices.size() / 3 + cell[i]);
                    }
                }
            }

            uint32_t i = 0;
            for (const auto& p: grid) {
                glm::vec3 a = {p.x, p.y, KEY};
                glm::vec3 new_a = T_inv * a;
                mesh_vertices.push_back(new_a.x);
                mesh_vertices.push_back(new_a.y);
                mesh_vertices.push_back(new_a.z);

                mesh_normals.push_back(face.normal[0]);
                mesh_normals.push_back(face.normal[1]);
                mesh_normals.push_back(face.normal[2]);


                for (const auto& t: triangles) {
                    /*HATE_ERROR_F(
                            "Triangle: %f %f, %f %f, %f %f", t.points[0].x, t.points[0].y,
                            t.points[1].x, t.points[1].y, t.points[2].x, t.points[2].y
                    );
                    HATE_INFO_F("Point: %f %f", p.x, p.y);*/

                    /*glm::vec2 stop_point = {-52.851002, -2.999996};
                    if (std::abs(p.x - stop_point.x) < 0.01f and
                        std::abs(p.y - stop_point.y) < 0.01f) {
                        bool on_edge = isPointOnTriangleEdge(p, t.points);
                        bool in_polygon = isPointInTriangle(p, t.points);
                        HATE_INFO_F("on_edge: %d, in_polygon: %d", on_edge, in_polygon);
                        //HATE_FATAL("STOP");
                    }*/

                    if (isPointInTriangle(p, t.points) or isPointOnTriangleEdge(p, t.points)) {

                        glm::vec2 point = p;


                        glm::vec2 v1 = t.points[0];
                        glm::vec2 vp = point - v1;

                        // Вычисляем скалярные произведения
                        float d20 = glm::dot(vp, t.v0);
                        float d21 = glm::dot(vp, t.v1v3);

                        // Вычисляем барицентрические координаты
                        float v = (t.v1v3.y * d20 - t.v0.y * d21) * t.invDenom;
                        float w = (t.v0.x * d21 - t.v1v3.x * d20) * t.invDenom;
                        float u = 1.0f - v - w;

                        glm::vec3 bary_coords = ComputeBarycentricCoordinates(
                                p, t.points[0], t.points[1], t.points[2]
                        );

                        glm::vec2 uv = bary_coords.x * t.tex[0] + bary_coords.y * t.tex[1] +
                                       bary_coords.z * t.tex[2];

                        /*HATE_INFO_F(
                                "Bary coords: %f %f %f", bary_coords.x, bary_coords.y, bary_coords.z
                        );*/

                        mesh_UVs.push_back(uv.x);
                        mesh_UVs.push_back(uv.y);

                        // HATE_WARNING("Added");
                        break;
                    }

                    bool on_p0 = std::abs(p.x - t.points[0].x) < 0.01f and
                                 std::abs(p.y - t.points[0].y) < 0.01f;
                    bool on_p1 = std::abs(p.x - t.points[1].x) < 0.01f and
                                 std::abs(p.y - t.points[1].y) < 0.01f;
                    bool on_p2 = std::abs(p.x - t.points[2].x) < 0.01f and
                                 std::abs(p.y - t.points[2].y) < 0.01f;

                    if (on_p0 or on_p1 or on_p2) {
                        if (on_p0) {
                            mesh_UVs.push_back(t.tex[0].x);
                            mesh_UVs.push_back(t.tex[0].y);
                        } else if (on_p1) {
                            mesh_UVs.push_back(t.tex[1].x);
                            mesh_UVs.push_back(t.tex[1].y);
                        } else {
                            mesh_UVs.push_back(t.tex[2].x);
                            mesh_UVs.push_back(t.tex[2].y);
                        }
                        // HATE_WARNING("Added Vertex");
                        break;
                    }


                    std::array<std::array<glm::vec2, 4>, 3> edges = {
                            {{t.points[0], t.points[1], t.tex[0], t.tex[1]},
                             {t.points[1], t.points[2], t.tex[1], t.tex[2]},
                             {t.points[2], t.points[0], t.tex[2], t.tex[0]}}
                    };

                    /*bool stop = false;
                    for (uint32_t i = 3; i < 3; ++i) {
                        glm::vec2 v1 = edges[i][0];
                        glm::vec2 v2 = edges[i][1];
                        glm::vec2 min = glm::min(v1, v2);
                        glm::vec2 max = glm::max(v1, v2);

                        if (std::abs(p.x - v1.x) < 0.01f and std::abs(v1.x - v2.x) < 0.01f) {
                            if (p.y > min.y and p.y < max.y) {
                                glm::vec2 t0 = edges[i][2];
                                glm::vec2 t1 = edges[i][3];

                                float l = v2.y - v1.y;
                                float procent = (p.y - v1.y) / l;

                                float x = t0.x + (t1.x - t0.x) * procent;

                                mesh_UVs.push_back(x);
                                mesh_UVs.push_back(t0.y);
                                //HATE_WARNING("Added Edge V");
                                stop = true;
                                break;
                            }
                        } else if (std::abs(p.y - v1.y) < 0.01f and std::abs(v1.y - v2.y) < 0.01f) {
                            if (p.x > min.x and p.x < max.x) {
                                // TODO: Horizontal
                                // return true;
                                // float m = (max.y - min.y) / (max.x - min.x);
                                // float y =
                                glm::vec2 t0 = edges[i][2];
                                glm::vec2 t1 = edges[i][3];

                                float l = v2.x - v1.x;
                                float procent = (p.x - v1.x) / l;

                                float y = t0.y + (t1.y - t0.y) * procent;

                                mesh_UVs.push_back(t0.x);
                                mesh_UVs.push_back(y);
                                //HATE_WARNING("Added Edge H");
                                stop = true;
                                break;
                            }
                        } else if (p.y > min.y and p.y < max.y and p.x > min.x and p.x < max.x) {
                            float m = (max.y - min.y) / (max.x - min.x);
                            float b = p.x * m - min.y;

                            //HATE_INFO_F("Point: %f %f, b: %f", p.x, p.y, b);
                            if (std::abs(p.y - b) < 0.01f) {
                                // TODO: Diagonal
                                stop = true;
                                break;
                            }
                        }
                    }

                    if (stop)
                        break;*/
                }


                /*glm::vec2 uv = {
                        start_tex.x + (p.x - poly_min.x) * UV_Vertex_K.x,
                        start_tex.y + (p.y - poly_min.y) * UV_Vertex_K.y
                };
                //glm::vec2 uv = p;
                mesh_UVs.push_back(uv.x);
                mesh_UVs.push_back(uv.y);
                i++;*/
            }
        }

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
        mesh->setName(obj.name);
        mesh->setPosition(center);
        mesh->setUV(mesh_UVs);
        bindObj(mesh);

        if (obj.material != "" and this->materials.count(obj.material) > 0) {
            mesh->setTexture(&this->textures[this->materials[obj.material].texture_id]);
        }
        //  mesh->disableLightShading();

        meshes.push_back(mesh);
    }
    return meshes;
}


// Function to sort vertices in counter clockwise order (thanks ChatGPT)
inline static void sortVerticesCCW(std::vector<glm::vec3>* vertices, const glm::vec3& normal) {
    // Calculate centroid
    glm::vec3 centroid(0.0f);
    for (const auto& v: *vertices) {
        centroid += v;
    }
    centroid /= static_cast<float>(vertices->size());

    // Find two orthogonal vectors
    glm::vec3 up = (fabs(normal.x) > 0.0001f) ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);
    glm::vec3 tangent = glm::normalize(glm::cross(up, normal));
    glm::vec3 bitangent = glm::normalize(glm::cross(normal, tangent));

    // Lambda function to calculate angle
    auto calculateAngle = [&](const glm::vec3& vertex) {
        glm::vec3 relative = vertex - centroid;
        float x = glm::dot(relative, tangent);
        float y = glm::dot(relative, bitangent);
        return std::atan2(y, x); // Угол в 2D
    };

    // Sort vertices by angles
    std::sort(vertices->begin(), vertices->end(), [&](const glm::vec3& a, const glm::vec3& b) {
        return calculateAngle(a) > calculateAngle(b);
    });
}

void ObjMapModel::generateCollision(
        std::vector<glm::vec3>& vertices, std::vector<ObjObject>& objects
) {
    // Collision generation
    for (auto& obj: objects) {
        std::vector<float> obj_vertices;
        std::vector<std::vector<uint32_t>> obj_faces_indices;

        std::unordered_map<uint32_t, uint32_t> vertex_map;
        for (auto& face: obj.faces) {
            std::vector<uint32_t> face_indices;

            std::vector<glm::vec3> sorted_vertices; // Vector for sort in CCW
            std::vector<glm::vec3> norm_vertices; // Original vertices
            std::vector<uint32_t> norm_indices; // 'Map' alternate
            for (auto& index: face.indices) {
                sorted_vertices.push_back(vertices[index]);
                norm_vertices.push_back(vertices[index]);
                norm_indices.push_back(index);
            }
            sortVerticesCCW(&sorted_vertices, face.normal);

            face.indices.clear();
            for (auto& vertex: sorted_vertices) {
                auto it = std::find(norm_vertices.begin(), norm_vertices.end(), vertex);
                face.indices.push_back(norm_indices[it - norm_vertices.begin()]);
            }

            for (auto& index: face.indices) {
                if (vertex_map.count(index) == 0) {
                    vertex_map[index] = obj_vertices.size() / 3;
                    obj_vertices.push_back(vertices[index].z);
                    obj_vertices.push_back(vertices[index].y);
                    obj_vertices.push_back(vertices[index].x);
                    face_indices.push_back(vertex_map[index]);

                } else {
                    face_indices.push_back(vertex_map[index]);
                }
            }
            obj_faces_indices.push_back(face_indices);
        }

        convex_shapes.push_back(
                ConvexShape(obj_vertices, obj_faces_indices, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0))
        );
    }

    for (auto& shape: convex_shapes) {
        static_body.addCollisionShapeRef(&shape);
    }
}


StaticBody* ObjMapModel::getStaticBody() {
    return &static_body;
}


/*==================================> STRING FUNCS <=========================================*/

std::vector<std::string> split(const std::string& str, const std::string& delimiters) {
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = 0;

    while ((end = str.find_first_of(delimiters, start)) != std::string::npos) {
        if (end > start)
            tokens.push_back(str.substr(start, end - start)); // Extract token
        start = end + 1; // Move past the delimiter
    }
    if (start < str.length()) {
        tokens.push_back(str.substr(start)); // Add the last token
    }

    return tokens;
}

void trim(std::string& str) {
    str.erase(str.find_last_not_of(" \t\n\v\r") + 1);
    str.erase(0, str.find_first_not_of(" \t\n\v\r"));
}
