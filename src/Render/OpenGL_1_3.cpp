#include <chrono>
#include <cstdint>
#include <glad/gl.h>

#include <HateEngine/Render/OpenGL_1_3.hpp>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <HateEngine/Log.hpp>
#include <HateEngine/Objects/GLTFAnimationPlayer.hpp>
#include <HateEngine/Objects/Light/Light.hpp>
#include <HateEngine/Objects/Light/SpotLight.hpp>
#include <HateEngine/Objects/Mesh.hpp>
#include <HateEngine/Resources/Model.hpp>
#include "HateEngine/Objects/Interfaces/Renderable3DInterface.hpp"
#include "HateEngine/Objects/Object.hpp"
#include "glm/ext.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/matrix.hpp"

// Include OpenGL Utility header (glu.h)
#ifdef __linux__
#include <GL/glu.h>
#elif __APPLE__
#include <OpenGL/glu.h>
#elif _WIN32
#include <windows.h>
#include <GL/glu.h>
#endif

using namespace HateEngine;

OpenGL_1_3::OpenGL_1_3(Engine* engine) {
    this->engine = engine;

    GLfloat mat_specular[] = {0.0, 0.0, 0.0, 0.0};
    GLfloat mat_shininess[] = {0.0};
    GLfloat light_position[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
    glClearColor(0.0, 1.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    GLfloat light_ambient[] = {1.0, 1.0, 1.0, 1.0};


    // GLfloat mat_specular[] = {0.3f, 0.3f, 0.3f, 1.0f};
    // GLfloat mat_shininess[] = {25.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);


    /*glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
    glLightfv(GL_LIGHT0,GL_POSITION,light_position);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.2);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.5);*/
    // glLightfv(GL_LIGHT0,GL_DIFFUSE,white_light);
    // glLightfv(GL_LIGHT0,GL_SPECULAR,white_light);
    // auto a = GL_CLAMP_TO_EDGE

    // glEnableClientState(GL_COLOR_ARRAY);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_BLEND);
    // glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDisable(GL_BLEND);
    // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //  glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    glActiveTexture(GL_TEXTURE1);
    glClientActiveTexture(GL_TEXTURE1);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    // glDisable(GL_FOG);

    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);
    initNuklearUI();
}

//=======================> 3D <========================

static uint32_t fog_modes[] = {GL_LINEAR, GL_EXP, GL_EXP2};

uint32_t fr_texture1 = 0;

#define render_width 800
#define render_height 600

void OpenGL_1_3::Render() {
    HateEngine::Level* level = this->engine->getLevel();
    glm::vec4 backgroundColor = level->getBackgroundColor();
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = this->engine->getResolution();
    glm::vec2 displayScale = this->engine->getDisplayScale();
    glViewport(0, 0, resolution.x * displayScale.x, resolution.y * displayScale.y);

    glEnable(GL_NORMALIZE);


    // Fog
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, fog_modes[level->getFogMode()]);
    glFogf(GL_FOG_DENSITY, level->getFogDensity());
    glFogf(GL_FOG_START, level->getFogStart());
    glFogf(GL_FOG_END, level->getFogEnd());
    glm::vec4 fogColor = level->getFogColor();
    glFogfv(GL_FOG_COLOR, &fogColor.r);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &level->calculatedAmbientLight.r);


    if (level->camera != nullptr)
        this->Draw3D(level->camera, &level->renderable_objects, &level->light);
    glDisable(GL_FOG);


    this->DrawNuklearUI(&level->ui_widgets);

    last_gpu_time = gpu_time;
    gpu_time = 0;
}

void OpenGL_1_3::renderMesh(const Mesh* mesh) {
    if (mesh->getCorrectTransparency())
        corrent_transparent_buffer_meshes.push_back(mesh);
    else
        rawRenderMesh(mesh);
}


inline Camera* OpenGL_1_3::getCamera() {
    return this->engine->getLevel()->camera;
}


void OpenGL_1_3::Draw3D(
        Camera* camera, std::unordered_map<UUID, Renderable3DInterface*>* renderable,
        std::unordered_map<UUID, Light*>* lights
) {
    glPushMatrix();
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    renderCamera(camera);
    if (camera->isSkyBoxEnabled()) {
        std::vector<Light*> n;
        // render(camera->getSkyBox(), &n);
    }
    glEnable(GL_LIGHTING);
    glDisable(GL_LIGHT0);


    this->draw_calls = 0;
    glm::mat4 view = camera->getGlobalRotationMatrix();
    glm::vec3 cam_pos = camera->getGlobalPosition();
    view = camera->getViewMatrix();
    frustrum.update(camera->getProjectionMatrix(this->engine->getAspectRatio()) * view);

    auto t0 = std::chrono::high_resolution_clock::now();
    for (const auto& r: *renderable) {
        r.second->render(this);
    }
    auto t1 = std::chrono::high_resolution_clock::now();

    auto dt = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
    // gpu_time = dt;


    std::sort(
            corrent_transparent_buffer_meshes.begin(), corrent_transparent_buffer_meshes.end(),
            [&cam_pos](const Mesh* a, const Mesh* b) {
                return a->getAABBDistanceToPoint(cam_pos) > b->getAABBDistanceToPoint(cam_pos);
                /*return glm::distance(a->getGlobalPosition(), cam_pos) >
                       glm::distance(b->getGlobalPosition(), cam_pos);*/
            }
    );


    for (auto& r: corrent_transparent_buffer_meshes) {
        rawRenderMesh(r);
    }

    corrent_transparent_buffer_meshes.clear();


    glPopMatrix();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
}


inline void OpenGL_1_3::rawRenderMesh(const Mesh* mesh, bool skip_transparency_check) {
    if (!frustrum.isBoxVisible(
                mesh->getAABBMin() + mesh->getGlobalPosition(),
                mesh->getAABBMax() + mesh->getGlobalPosition()
        ))
        return;

    // next_attrib = attribs;

    ++this->draw_calls;

    glPushMatrix();

    bool face_culling_disabled = false;
    if (!mesh->getFaceCulling()) {
        glDisable(GL_CULL_FACE);
        face_culling_disabled = true;
    }

    std::vector<Light*> lights_vec;
    if (mesh->isLightShading()) {
        float max_light_render_dist = mesh->getCustomMaxLightDist();
        if (max_light_render_dist == 0)
            max_light_render_dist = this->maxLightRenderDist;
        lights_vec = getNearestLights(mesh, max_light_render_dist);
        renderLight(&lights_vec);
    }

    auto time_start = std::chrono::high_resolution_clock::now();


    // I think, some if-branches doesn't have big GPU time impact


    bool textures_disabled = false;
    bool light_textures_disabled = false;
    if (mesh->getTexture() != nullptr and mesh->getTexture()->is_loaded) {
        glActiveTexture(GL_TEXTURE0);
        if (not mesh->getTexture()->is_gpu_loaded)
            mesh->getTexture()->Load(loadTexture, unloadTexture);

        glBindTexture(GL_TEXTURE_2D, mesh->getTexture()->getTextureID());
        glClientActiveTexture(GL_TEXTURE0);
        glTexCoordPointer(2, GL_FLOAT, 0, mesh->getUV()->data());


    } else {
        textures_disabled = true;
        glActiveTexture(GL_TEXTURE0);
        glClientActiveTexture(GL_TEXTURE0);
        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    if (not textures_disabled and mesh->getLightTexture() != nullptr and
        mesh->getLightTexture()->is_loaded) {
        glActiveTexture(GL_TEXTURE1);
        if (not mesh->getLightTexture()->is_gpu_loaded)
            mesh->getLightTexture()->Load(loadTexture, unloadTexture);

        glBindTexture(GL_TEXTURE_2D, mesh->getLightTexture()->getTextureID());
        glClientActiveTexture(GL_TEXTURE1);
        glTexCoordPointer(2, GL_FLOAT, 0, mesh->getLightUV()->data());

    } else {
        light_textures_disabled = true;
        glActiveTexture(GL_TEXTURE1);
        glClientActiveTexture(GL_TEXTURE1);
        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    glm::vec3 par_pos = mesh->getGlobalPosition();
    glTranslatef(par_pos.x, par_pos.y, par_pos.z);
    glMultMatrixf(glm::value_ptr(mesh->getGlobalRotationMatrix()));

    glm::vec3 scale = mesh->getGlobalScale();
    glScalef(scale.x, scale.y, scale.z);

    glVertexPointer(3, GL_FLOAT, 0, mesh->getVertices()->data());
    glNormalPointer(GL_FLOAT, 0, mesh->getNormals()->data());
    glDrawElements(
            GL_TRIANGLES, mesh->getIndicies()->size(), GL_UNSIGNED_INT, mesh->getIndicies()->data()
    );

    /* Restoring OpenGL settings */
    if (face_culling_disabled) {
        glEnable(GL_CULL_FACE);
    }

    if (light_textures_disabled) {
        glActiveTexture(GL_TEXTURE1);
        glClientActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    if (textures_disabled) {
        glActiveTexture(GL_TEXTURE0);
        glClientActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    for (uint32_t i = 0; i < lights_vec.size(); ++i) {
        glDisable(GL_LIGHT0 + i);
    }

    glPopMatrix();

    auto time_end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(time_end - time_start).count();
    gpu_time += time;
}


inline void OpenGL_1_3::renderCamera(Camera* camera) {
    glm::mat4 Mp = camera->getProjectionMatrix(this->engine->getAspectRatio());
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(Mp));

    glm::mat4 mat = camera->getViewMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(mat));
}

inline void OpenGL_1_3::renderLight(std::vector<Light*>* lights_vec) {
    // std::cout << "Lights: " << lights_vec->size() << "\n";
    for (int i = 0; i < lights_vec->size(); ++i) {
        int light_num = GL_LIGHT0 + i;
        Light* light = (*lights_vec)[i];
        glm::vec3 pos = light->getGlobalPosition();
        glm::vec3 att = light->getAttenuation();

        glEnable(light_num);
        glLightfv(light_num, GL_DIFFUSE, glm::value_ptr(light->getColor()));
        glLightf(light_num, GL_CONSTANT_ATTENUATION, att.x);
        glLightf(light_num, GL_LINEAR_ATTENUATION, att.y);
        glLightf(light_num, GL_QUADRATIC_ATTENUATION, att.z);

        glm::vec4 l_position = glm::vec4(pos, 1.0f);
        if (light->getLightType() == Light::LightTypeEnum::DirectionalLight)
            l_position = glm::vec4(light->getDirection() * -1.0f, 0.0f);
        glLightfv(light_num, GL_POSITION, glm::value_ptr(l_position));

        if (light->getLightType() == Light::SpotLight) {
            SpotLight* spot_light = (SpotLight*) light;
            glm::vec3 dir = light->getDirection();
            glLightfv(light_num, GL_SPOT_DIRECTION, glm::value_ptr(dir));
            glLightf(light_num, GL_SPOT_CUTOFF, spot_light->angleCutoff);
            glLightf(light_num, GL_SPOT_EXPONENT, spot_light->exponent);
        }
    }
}

struct LightDistSt {
    float length;
    Light* light;
};

inline std::vector<Light*> OpenGL_1_3::getNearestLights(
        const Mesh* mesh, float max_light_render_dist
) {
    std::vector<Light*> result;
    result.reserve(this->lights->size());

    std::vector<LightDistSt> light_dist;
    light_dist.reserve(lights->size());

    /*for (int i = 0; i < lights_vec->size() and result.size() <= maxLightCount; ++i) {
        Light* light = (*lights_vec)[i];
        if (!light->getGlobalVisible())
            continue;
        if (light->getLightType() == Light::LightTypeEnum::DirectionalLight)
            result.push_back(i);
        else
            light_dist.push_back({mesh->getAABBDistanceToPoint(light->getGlobalPosition()), i});
    }*/

    /*if (this->lights->size() <= this->maxLightCount) {
        for (auto& light_pair : *lights) {
            Light* light = light_pair.second;
            if (!light->getGlobalVisible())
                continue;
            result.push_back(light);
        }
        return result;
    }*/

    for (auto& light_pair: *lights) {
        Light* light = light_pair.second;
        if (!light->getGlobalVisible())
            continue;
        if (light->getLightType() == Light::LightTypeEnum::DirectionalLight)
            result.push_back(light);
        else
            light_dist.push_back({mesh->getAABBDistanceToPoint(light->getGlobalPosition()), light});
    }

    // if
    // TODO: Optimize

    std::sort(light_dist.begin(), light_dist.end(), [](LightDistSt& a, LightDistSt& b) {
        return a.length < b.length;
    });
    for (int i = 0; i < light_dist.size() and result.size() <= maxLightCount; ++i) {
        if (light_dist[i].length <= max_light_render_dist)
            result.push_back(light_dist[i].light);
    }
    return result;
}


#define CASE(cond, body)                                                                           \
    case cond:                                                                                     \
        body;                                                                                      \
        break
void OpenGL_1_3::loadTexture(Texture* texture_ptr) {
    int texWrap;
    switch (texture_ptr->texWrap) {
        CASE(Texture::TexWrap::Clamp, texWrap = GL_CLAMP);
        CASE(Texture::TexWrap::Repeat, texWrap = GL_REPEAT);
        CASE(Texture::TexWrap::ClampToEdge, texWrap = GL_CLAMP_TO_EDGE);
        CASE(Texture::TexWrap::ClampToBorder, texWrap = GL_CLAMP_TO_BORDER);
        default:
            texWrap = GL_REPEAT;
    }

    int texFiltering;
    switch (texture_ptr->texFiltering) {
        CASE(Texture::TexFiltering::Nearest, texFiltering = GL_NEAREST);
        CASE(Texture::TexFiltering::Linear, texFiltering = GL_LINEAR);
        default:
            texFiltering = GL_LINEAR;
    }

    int texMipMapFiltering;
    if (texture_ptr->texMipMapEnabled) {
        switch (texture_ptr->texMipMapFiltering) {
            CASE(Texture::TexFiltering::Nearest, texMipMapFiltering = GL_NEAREST_MIPMAP_LINEAR);
            CASE(Texture::TexFiltering::Linear, texMipMapFiltering = GL_LINEAR_MIPMAP_LINEAR);
            default:
                texMipMapFiltering = GL_LINEAR;
        }
    } else {
        texMipMapFiltering = texFiltering;
    }

    int textureFormat;
    switch (texture_ptr->textureFormat) {
        CASE(Texture::TexType::RGB, textureFormat = GL_RGB);
        CASE(Texture::TexType::RGBA, textureFormat = GL_RGBA);
        default:
            textureFormat = GL_RGB;
    }

    glGenTextures(1, &texture_ptr->textureGL_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ptr->textureGL_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texFiltering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texMipMapFiltering);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, texture_ptr->MipMapLodBias);

    // #define in GL_COMPRESSED_RGB
    // #define in GL_R3_G3_B2
    // #define in GL_RGB
    if (texture_ptr->texMipMapEnabled) // MipMap enabled
        gluBuild2DMipmaps(
                GL_TEXTURE_2D, textureFormat, texture_ptr->width, texture_ptr->height,
                textureFormat, GL_UNSIGNED_BYTE, texture_ptr->data.data()
        );
    else // MipMad disabled
        glTexImage2D(
                GL_TEXTURE_2D, 0, textureFormat, texture_ptr->width, texture_ptr->height, 0,
                textureFormat, GL_UNSIGNED_BYTE, texture_ptr->data.data()
        );

    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGL_1_3::unloadTexture(Texture* texture_ptr) {
    glDeleteTextures(1, &texture_ptr->textureGL_ID);
    texture_ptr->textureGL_ID = 0;
}


void OpenGL_1_3::setMaxLightRenderDist(float dist) {
    this->maxLightRenderDist = dist;
}
//============================ End 3D ============================
