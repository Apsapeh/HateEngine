#include <HateEngine/Resources/ObjMapModel.hpp>
#include <HateEngine/Resources/Texture.hpp>
#include <HateEngine/Resources/HERFile.hpp>
#include <algorithm>
#include <cstdint>
#include <fstream>
#include <cstdio>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>

#include "HateEngine/Log.hpp"
#include "HateEngine/Objects/Object.hpp"
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
        std::string obj_filename, std::string map_file_name, std::string lightmap_file_name,
        float grid_size, bool generate_collision, float lod_dist, float lod_step
) {
    bindObj(&static_body);
    this->generate_collision = generate_collision;
    this->obj_file_name = obj_filename;
    this->map_file_name = map_file_name;
    // get parrent from obj_filename
    size_t pos = obj_filename.find_last_of("/\\");
    if (pos != std::string::npos)
        obj_file_path = obj_filename.substr(0, pos + 1);

    // get folder .heluv
    pos = lightmap_file_name.find_last_of("/\\");
    if (pos != std::string::npos)
        heluv_file_path = lightmap_file_name.substr(0, pos + 1);

    if (!lightmap_file_name.empty()) {
        FILE* file = fopen(lightmap_file_name.c_str(), "rb");
        if (file) {
            fseek(file, 0, SEEK_END);
            long file_size = ftell(file);
            rewind(file);

            std::vector<uint8_t> buffer;
            buffer.resize(file_size);

            size_t read_size = fread(buffer.data(), 1, file_size, file);

            fclose(file);
            parseHeluv(buffer);
        } else {
            HATE_ERROR_F("Failed to open file: %s", lightmap_file_name.c_str());
        }


        /*std::ifstream lightmap_file(lightmap_file_name);
        if (lightmap_file.is_open()) {
            std::vector<uint8_t> lightmap_data(
                    (std::istreambuf_iterator<char>(lightmap_file)),
                    (std::istreambuf_iterator<char>())
            );
            parseHeluv(lightmap_data);
        } else
            */
    }

    std::ifstream file(obj_filename);
    if (!file.is_open()) {
        HATE_ERROR_F("Failed to open file: %s", obj_filename.c_str());
        return;
    }

    std::string data((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
    parseObj(data, grid_size, lod_dist, lod_step);


    if (!map_file_name.empty()) {
        std::ifstream map_file(map_file_name);
        if (map_file.is_open()) {
            std::string map_data(
                    (std::istreambuf_iterator<char>(map_file)), (std::istreambuf_iterator<char>())
            );
            parseMap(map_data, grid_size);
        } else
            HATE_ERROR_F("Failed to open file: %s", map_file_name.c_str());
    }

    is_loaded = true;
}


ObjMapModel::ObjMapModel(
        HERFile* her, std::string obj_file_data, std::string map_file_data,
        std::vector<uint8_t> heluv_data, float grid_size, bool generate_collision, float lod_dist,
        float lod_step

) {
    bindObj(&static_body);
    this->generate_collision = generate_collision;
    if (!heluv_data.empty())
        parseHeluv(heluv_data);
    parseObj(obj_file_data, grid_size, lod_dist, lod_step, her);
    if (!map_file_data.empty())
        parseMap(map_file_data, grid_size);
    is_loaded = true;
}

ObjMapModel::~ObjMapModel() {
    if (entities_data_deleter != nullptr)
        entities_data_deleter(entities_data);

    for (auto& l: heluv) {
        delete l.second.texture;
    }
}

void ObjMapModel::deserializeEntities(
        const std::unordered_map<std::string, EntityDeserialzer>& entity_deserializers, void* data,
        void (*data_deleter)(void*)
) {
    for (auto& entity: entities) {
        if (entity_deserializers.count(entity.classname))
            entity_deserializers.at(entity.classname)(this, entity, data);
    }

    entities_data = data;

    if (data_deleter != nullptr)
        this->entities_data_deleter = data_deleter;
}

void ObjMapModel::addEntityObjectToLevel(Mesh* object) {
    add_to_level_meshes.push_back(object);
}
void ObjMapModel::addEntityObjectToLevel(BillboardMesh* object) {
    add_to_level_billboards.push_back(object);
}
void ObjMapModel::addEntityObjectToLevel(Light* object) {
    add_to_level_lights.push_back(object);
}
void ObjMapModel::addEntityObjectToLevel(Model* object) {
    add_to_level_models.push_back(object);
}
void ObjMapModel::addEntityObjectToLevel(ObjMapModel* object) {
    add_to_level_objMapModels.push_back(object);
}
void ObjMapModel::addEntityObjectToLevel(GLTFAnimationPlayer* object) {
    add_to_level_animationPlayers.push_back(object);
}
void ObjMapModel::addEntityObjectToLevel(Particles* object) {
    add_to_level_particles.push_back(object);
}

/*======================================> PARSERS <==============================================*/
void ObjMapModel::parseHeluv(std::vector<uint8_t>& data, HERFile* her) {
    uint8_t* ptr = data.data();
    uint32_t version = *(uint32_t*) ptr;
    ptr += 4;

    if (version < 2) {
        HATE_ERROR_F("Heluv version %d is not supported, version 2 or higher is required", version);
        return;
    }

    uint32_t obj_count = *(uint32_t*) ptr;
    ptr += 4;

    uint8_t image_format = *ptr; // 0 = RAW, 1 = PNG
    ptr += 1;

    for (uint32_t i = 0; i < obj_count; i++) {
        uint16_t name_len = *(uint16_t*) ptr;
        ptr += 2;
        uint32_t faces_count = *(uint32_t*) ptr;
        ptr += 4;
        uint32_t data_size = *(uint32_t*) ptr;
        ptr += 4;

        std::string name((char*) ptr, name_len);
        ptr += name_len;

        /*std::string tex_filename = this->heluv_file_path + name + ".png";
        HateEngine::Texture* tex;
        if (her != nullptr) {
            tex = new HateEngine::Texture(
                    (*her)[tex_filename].asTexture(Texture::Repeat, Texture::Linear, false)
            );
        } else
            tex = new HateEngine::Texture(tex_filename, Texture::Repeat, Texture::Linear, false);*/

        std::vector<std::vector<glm::vec2>> faces;
        for (uint32_t j = 0; j < faces_count; j++) {
            uint8_t uvs_count = *ptr;
            ptr += 1;

            std::vector<glm::vec2> uv;
            for (uint8_t k = 0; k < uvs_count; k++) {
                float u = *(float*) ptr;
                ptr += 4;
                float v = *(float*) ptr;
                ptr += 4;
                uv.push_back(glm::vec2(u, v));
            }

            faces.push_back(uv);
        }

        std::vector<uint8_t> tex_data;
        tex_data.resize(data_size);
        memcpy(tex_data.data(), ptr, data_size);
        ptr += data_size;

        Texture* tex = new HateEngine::Texture(tex_data, Texture::Repeat, Texture::Linear, false);
        heluv[name] = {faces, tex};
    }
    int a = 5;
}


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
            std::vector<glm::vec2> light_tex;
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

                if (this->heluv.count(current_obj->name)) {
                    uint32_t face_index = current_obj->faces.size();
                    uint32_t uv_index = i - 1;

                    if (face_index < this->heluv[current_obj->name].uv.size() and
                        uv_index < this->heluv[current_obj->name].uv[face_index].size()) {
                        light_tex.push_back(this->heluv[current_obj->name].uv[face_index][uv_index]
                        );
                    } else {
                        light_tex.push_back(glm::vec2(0, 0));
                    }
                } else {
                    light_tex.push_back(glm::vec2(0, 0));
                }
            }

            ObjFace face;
            face.indices = v_indices;
            face.tex_indices = t_indices;
            face.light_tex = light_tex;
            if (n_indices.size() > 0) {
                face.normal[0] = normals[n_indices[0]].x;
                face.normal[1] = normals[n_indices[0]].y;
                face.normal[2] = normals[n_indices[0]].z;
            }

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


void ObjMapModel::parseMap(std::string data, float grid_size) {
    Entity current_entity;

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
                // New entity
                // entities.push_back(Entity());
                // current_entity = &entities[entities.size() - 1];
                current_entity = Entity();
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

            if (!current_entity.classname.empty() and current_entity.classname != "worldspawn")
                entities.push_back(current_entity);
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
                continue;
            }

            trim(words[0]);
            trim(words[1]);
            // HATE_ERROR_F("Word_0: %s, Word_1: %s", words[0].c_str(), words[1].c_str());

            if (words[0] == "classname") {
                current_entity.classname = words[1];
            } else if (words[0] == "origin") {
                Property prop(words[1]);
                current_entity.position = prop.asVec3XZY() / grid_size;
            } else {
                current_entity.properties.insert({words[0], Property(words[1])});
            }
        }
    }
}


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
        return glm::vec3(-1.0f, -1.0f, -1.0f); // Возвращаем ошибочные координаты
    }

    return glm::vec3(u, v, w);
}


struct Triangle {
    std::vector<glm::vec2> points;
    glm::vec2 tex[3] = {glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)};
    glm::vec2 light_tex[3] = {glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)};
};

void add_vertex_and_uvs(
        glm::vec3 new_a, glm::vec2 p, std::vector<Triangle>& triangles,
        std::vector<float>& mesh_vertices, std::vector<float>& mesh_UVs,
        std::vector<float>& mesh_lightmap_UVs
) {
    mesh_vertices.push_back(new_a.x);
    mesh_vertices.push_back(new_a.y);
    mesh_vertices.push_back(new_a.z);

    for (const auto& t: triangles) {
        if (isPointInTriangle(p, t.points) or isPointOnTriangleEdge(p, t.points)) {
            glm::vec3 bary_coords =
                    ComputeBarycentricCoordinates(p, t.points[0], t.points[1], t.points[2]);

            glm::vec2 uv =
                    bary_coords.x * t.tex[0] + bary_coords.y * t.tex[1] + bary_coords.z * t.tex[2];

            glm::vec2 light_uv = bary_coords.x * t.light_tex[0] + bary_coords.y * t.light_tex[1] +
                                 bary_coords.z * t.light_tex[2];

            mesh_UVs.push_back(uv.x);
            mesh_UVs.push_back(uv.y);
            mesh_lightmap_UVs.push_back(light_uv.x);
            mesh_lightmap_UVs.push_back(light_uv.y);
            break;
        }

        bool on_p0 =
                std::abs(p.x - t.points[0].x) < 0.01f and std::abs(p.y - t.points[0].y) < 0.01f;
        bool on_p1 =
                std::abs(p.x - t.points[1].x) < 0.01f and std::abs(p.y - t.points[1].y) < 0.01f;
        bool on_p2 =
                std::abs(p.x - t.points[2].x) < 0.01f and std::abs(p.y - t.points[2].y) < 0.01f;

        if (on_p0 or on_p1 or on_p2) {
            if (on_p0) {
                mesh_UVs.push_back(t.tex[0].x);
                mesh_UVs.push_back(t.tex[0].y);
                mesh_lightmap_UVs.push_back(t.light_tex[0].x);
                mesh_lightmap_UVs.push_back(t.light_tex[0].y);
            } else if (on_p1) {
                mesh_UVs.push_back(t.tex[1].x);
                mesh_UVs.push_back(t.tex[1].y);
                mesh_lightmap_UVs.push_back(t.light_tex[1].x);
                mesh_lightmap_UVs.push_back(t.light_tex[1].y);
            } else {
                mesh_UVs.push_back(t.tex[2].x);
                mesh_UVs.push_back(t.tex[2].y);
                mesh_lightmap_UVs.push_back(t.light_tex[2].x);
                mesh_lightmap_UVs.push_back(t.light_tex[2].y);
            }
            break;
        }
    }
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
        std::vector<float> mesh_lightmap_UVs;
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
            for (uint32_t i = 1; i < poly.size(); i++) {
                glm::vec2 p = poly[i];
                poly_min.x = std::min(p.x, poly_min.x);
                poly_min.y = std::min(p.y, poly_min.y);
                poly_max.x = std::max(p.x, poly_max.x);
                poly_max.y = std::max(p.y, poly_max.y);
            }

            // Triangulate the polygon


            std::vector<Triangle> triangles;
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
                Triangle t;
                t.points.push_back(poly[poly_ccw_idx[0]]);
                t.points.push_back(poly[poly_ccw_idx[i - 1]]);
                t.points.push_back(poly[poly_ccw_idx[i]]);

                if (!face.tex_indices.empty()) {
                    t.tex[0] = tex_coords[face.tex_indices[poly_ccw_idx[0]]];
                    t.tex[1] = tex_coords[face.tex_indices[poly_ccw_idx[i - 1]]];
                    t.tex[2] = tex_coords[face.tex_indices[poly_ccw_idx[i]]];
                }

                if (!face.light_tex.empty()) {
                    t.light_tex[0] = face.light_tex[poly_ccw_idx[0]];
                    t.light_tex[1] = face.light_tex[poly_ccw_idx[i - 1]];
                    t.light_tex[2] = face.light_tex[poly_ccw_idx[i]];
                }

                triangles.push_back(t);
            }

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
                        mesh_indicies.push_back(mesh_vertices.size() / 3);
                        glm::vec3 a = {grid[cell[0]].x, grid[cell[0]].y, KEY};
                        glm::vec3 new_a = T_inv * a;
                        add_vertex_and_uvs(
                                new_a, {a.x, a.y}, triangles, mesh_vertices, mesh_UVs,
                                mesh_lightmap_UVs
                        );

                        mesh_indicies.push_back(mesh_vertices.size() / 3);
                        a = {grid[cell[i - 1]].x, grid[cell[i - 1]].y, KEY};
                        new_a = T_inv * a;
                        add_vertex_and_uvs(
                                new_a, {a.x, a.y}, triangles, mesh_vertices, mesh_UVs,
                                mesh_lightmap_UVs
                        );

                        mesh_indicies.push_back(mesh_vertices.size() / 3);
                        a = {grid[cell[i]].x, grid[cell[i]].y, KEY};
                        new_a = T_inv * a;
                        add_vertex_and_uvs(
                                new_a, {a.x, a.y}, triangles, mesh_vertices, mesh_UVs,
                                mesh_lightmap_UVs
                        );

                        for (uint32_t i = 0; i < 3; i++) {
                            mesh_normals.push_back(face.normal[0]);
                            mesh_normals.push_back(face.normal[1]);
                            mesh_normals.push_back(face.normal[2]);
                        }
                    }
                }
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
        mesh->setLightUV(mesh_lightmap_UVs);
        bindObj(mesh);

        if (obj.material != "" and this->materials.count(obj.material) > 0) {
            mesh->setTexture(&this->textures[this->materials[obj.material].texture_id]);
        }

        if (heluv.count(obj.name)) {
            mesh->setLightTexture(heluv[obj.name].texture);
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
    size_t first = str.find_first_not_of(" \v\t\n");
    size_t last = str.find_last_not_of(" \v\t\n");
    str = str.substr(first, (last - first + 1));
}


/* ==== */
ObjMapModel::Property::Property(std::string value) : value(value) {
}

float ObjMapModel::Property::asFloat() const {
    return std::stof(value);
}

int ObjMapModel::Property::asInt() const {
    return std::stoi(value);
}

bool ObjMapModel::Property::asBool() const {
    return std::stoi(value);
}

std::string ObjMapModel::Property::asString() {
    return value;
}

glm::vec3 ObjMapModel::Property::asVec3() const {
    std::vector<std::string> values = split(value, " ");

    if (values.size() != 3) {
        HATE_ERROR_F("Invalid vec3: %s", value.c_str());
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    return glm::vec3(std::stof(values[0]), std::stof(values[1]), std::stof(values[2]));
}

glm::vec3 ObjMapModel::Property::asVec3XZY() const {
    glm::vec3 v = asVec3();
    return glm::vec3(v.x, v.z, -v.y);
}

glm::vec2 ObjMapModel::Property::asVec2() const {
    std::vector<std::string> values = split(value, " ");

    if (values.size() != 2) {
        HATE_ERROR_F("Invalid vec2: %s", value.c_str());
        return glm::vec2(0.0f, 0.0f);
    }

    return glm::vec2(std::stof(values[0]), std::stof(values[1]));
}
