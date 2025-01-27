#include <cstdint>
#include <glad/gl.h>

#include <HateEngine/Render/OpenGL15.hpp>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <HateEngine/Log.hpp>
#include <HateEngine/Objects/GLTFAnimationPlayer.hpp>
#include <HateEngine/Objects/Light/Light.hpp>
#include <HateEngine/Objects/Light/SpotLight.hpp>
#include <HateEngine/Objects/Mesh.hpp>
#include <HateEngine/Resources/Model.hpp>
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

OpenGL15::OpenGL15(Engine* engine) {
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
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    // glEnableClientState(GL_TEXTURE_COORD_ARRAY);

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

void OpenGL15::Render() {
    HateEngine::Level* level = this->engine->getLevel();
    glClearColor(
            level->backgroundColor.r, level->backgroundColor.g, level->backgroundColor.b,
            level->backgroundColor.a
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glViewport(0+, 0, render_width, render_height);
    glm::ivec2 resolution = this->engine->getResolution();
    glm::vec2 displayScale = this->engine->getDisplayScale();
    // glm::vec2 displayScale = {1, 1};
    glViewport(0, 0, resolution.x * displayScale.x, resolution.y * displayScale.y);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_NORMALIZE);


    // Fog
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, fog_modes[level->fogMode]);
    glFogf(GL_FOG_DENSITY, level->fogDensity);
    glFogf(GL_FOG_START, level->fogStart);
    glFogf(GL_FOG_END, level->fogEnd);
    glFogfv(GL_FOG_COLOR, &level->fogColor.r);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &level->calculatedAmbientLight.r);

    // glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    if (level->camera != nullptr)
        this->Draw3D(
                level->camera, &level->meshes, &level->models, &level->objMapModels, &level->animationPlayers,
                &level->particles, &level->lights
        );
    // glFlush();
    // glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glDisable(GL_FOG);

    glm::ivec2 window_size = this->engine->getResolution();
    /*if (window_size.x != render_width or window_size.y != render_height and
    false) { glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();




        glPushMatrix();


    glEnable(GL_TEXTURE_2D);
    if (fr_texture1 == 0) {
        glGenTextures(1, &fr_texture1);
        glBindTexture(GL_TEXTURE_2D, fr_texture1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 400, 300, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, render_width, render_height, 0,
    GL_RGB, GL_UNSIGNED_BYTE, 0);
    }
    //current_fr_texture = current_fr_texture == fr_texture1 ? fr_texture2 :
    fr_texture1; glBindTexture(GL_TEXTURE_2D, fr_texture1);
    //glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 400, 300, 0);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, render_width,
    render_height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, window_size.x, window_size.y);
    glDisableClientState(GL_COLOR_ARRAY);

    GLfloat vertices2[] = {
        1, 1,
        -1, 1,
        -1, -1,
        -1, -1,
        1, -1,
        1, 1
    };

    GLushort indices2[] = {
        0, 1, 2,
        3, 4, 5
    };

    GLfloat tex_coords2[] = {
        1, 1,
        0, 1,
        0, 0,
        0, 0,
        1, 0,
        1, 1
    };

    GLfloat colors2[] = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1,
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };

    //glDisable(GL_TEXTURE_2D);
    //glEnableClientState(GL_COLOR_ARRAY);

    //glColorPointer(3, GL_FLOAT, 0, colors2);
    glTexCoordPointer(2, GL_FLOAT, 0, tex_coords2);
    glVertexPointer(2, GL_FLOAT, 0, vertices2);


    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices2);



    glPopMatrix();
    }*/

    this->DrawNuklearUI(&level->ui_widgets);
}


std::vector<Mesh*> correct_buffer = {};
void OpenGL15::Draw3D(
        Camera* camera, std::vector<Mesh*>* meshes, std::vector<Model*>* models, std::vector<ObjMapModel*>* obj_map_models,
        std::vector<GLTFAnimationPlayer*>* animation_players, std::vector<Particles*>* particles,
        std::vector<Light*>* lights
) {

    glEnable(GL_BLEND);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    renderCamera(camera);
    if (camera->isSkyBoxEnabled()) {
        std::vector<Light*> n;
        render(camera->getSkyBox(), &n);
    }
    glEnable(GL_LIGHTING);
    glDisable(GL_LIGHT0);


    for (const auto s: *animation_players) {
        if (s->visible and s->isPlaying())
            for (const auto& mesh: *s->getMeshes()) {
                if (mesh->getCorrectTransparency())
                    correct_buffer.push_back(mesh);
                else
                    render(mesh, lights);
            }
    }

    for (auto* s: *particles) {
        for (const auto& particle: *s->getParticles()) {
            if (particle.getCorrectTransparency())
                correct_buffer.push_back((Mesh*) &particle);
            else
                render((const Mesh*) &particle, lights);
        }
    }

    for (const auto obj: *meshes) {
        if (obj->getCorrectTransparency())
            correct_buffer.push_back(obj);
        else
            render(obj, lights);
    }

    glm::vec3 pos = camera->getGlobalPosition();
    for (auto obj: *models) {
        if (obj->isLoaded()) {
            auto model_meshes = ((Model*) obj)->getLODMeshes();
            for (auto& mesh: *model_meshes) {
                const auto m = mesh.getMeshByPos(pos);
                if (m != nullptr) {
                    if (m->getCorrectTransparency())
                        correct_buffer.push_back(m);
                    else
                        render(m, lights);
                }
            }
        }
    }

    for (auto obj: *obj_map_models) {
        if (obj->isLoaded()) {
            auto model_meshes = obj->getLODMeshes(pos);
            if (model_meshes == nullptr)
                continue;
            for (auto& mesh: *model_meshes) {
                const auto m = mesh.getMeshByPos(pos);
                if (m != nullptr) {
                    if (m->getCorrectTransparency())
                        correct_buffer.push_back(m);
                    else
                        render(m, lights);
                }
            }
        }
    }

    // Render transparent objects in correct order
    if (not correct_buffer.empty()) {
        glm::vec3 camera_pos = camera->getGlobalPosition();
        std::sort(correct_buffer.begin(), correct_buffer.end(), [camera_pos](Mesh* a, Mesh* b) {
            return a->getAABBDistanceToPoint(camera_pos) > b->getAABBDistanceToPoint(camera_pos);
        });

        for (const auto obj: correct_buffer) {
            render(obj, lights);
        }

        correct_buffer.clear();
    }

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
}

void OpenGL15::render(const Mesh* mesh, std::vector<Light*>* lights_vec) {
    if (mesh->getGlobalVisible()) {
        glPushMatrix();
        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
        glPushAttrib(GL_ALL_ATTRIB_BITS);

        if (!mesh->getFaceCulling())
            glDisable(GL_CULL_FACE);

        std::vector<int> light_indicies;
        if (mesh->isLightShading()) {
            float max_light_render_dist = mesh->getCustomMaxLightDist();
            if (max_light_render_dist == 0)
                max_light_render_dist = this->maxLightRenderDist;
            light_indicies = getNearestLights(lights_vec, mesh, max_light_render_dist);
            renderLight(lights_vec, light_indicies);
        }

        glm::vec3 par_pos = mesh->getGlobalPosition();
        glTranslatef(par_pos.x, par_pos.y, par_pos.z);
        glMultMatrixf(glm::value_ptr(mesh->getGlobalRotationMatrix()));

        glm::vec3 scale = mesh->getGlobalScale();
        glScalef(scale.x, scale.y, scale.z);

        // Render Textures
        bool texture_enabled = false;
        bool light_texture_enabled = false;
        if (mesh->getTexture() != nullptr and mesh->getTexture()->is_loaded) {
            texture_enabled = true;
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glActiveTexture(GL_TEXTURE0);
            if (not mesh->getTexture()->is_gpu_loaded)
                mesh->getTexture()->Load(loadTexture, unloadTexture);

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, mesh->getTexture()->getTextureID());
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glClientActiveTexture(GL_TEXTURE0);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, 0, mesh->getUV()->data());
        }

        if (texture_enabled and mesh->getLightTexture() != nullptr and
            mesh->getLightTexture()->is_loaded) {
            light_texture_enabled = true;

            texture_enabled = true;
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glActiveTexture(GL_TEXTURE1);
            if (not mesh->getLightTexture()->is_gpu_loaded)
                mesh->getLightTexture()->Load(loadTexture, unloadTexture);

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, mesh->getLightTexture()->getTextureID());
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glClientActiveTexture(GL_TEXTURE1);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, 0, mesh->getLightUV()->data());
        }

        glVertexPointer(3, GL_FLOAT, 0, mesh->getVertices()->data());
        glNormalPointer(GL_FLOAT, 0, mesh->getNormals()->data());

        if (mesh->isColorEnabled()) {
            glEnable(GL_COLOR_MATERIAL);
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer((int) mesh->getColorChannels(), GL_FLOAT, 0, mesh->getColors()->data());
            glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        }

        // HATE_FATAL_F("Size: %d", mesh->getIndicies()->size());
        glDrawElements(
                GL_TRIANGLES, mesh->getIndicies()->size(), GL_UNSIGNED_INT,
                mesh->getIndicies()->data()
        );

        if (texture_enabled) {
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisable(GL_TEXTURE_2D);
        }


        glPopMatrix();
        glPopClientAttrib();
        glPopAttrib();
    }
}

inline void OpenGL15::renderCamera(Camera* camera) {
    glm::mat4 Mp = camera->getProjectionMatrix(this->engine->getAspectRatio());
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(Mp));

    glm::mat4 mat = camera->getViewMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(mat));
}

inline void OpenGL15::renderLight(
        std::vector<Light*>* lights_vec, const std::vector<int>& indicies
) {
    // std::cout << "Lights: " << lights_vec->size() << "\n";
    for (int i = 0; i < indicies.size(); ++i) {
        int index = indicies[i];
        int light_num = GL_LIGHT0 + i;
        Light* light = (*lights_vec)[index];
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
    int index;
};

inline std::vector<int> OpenGL15::getNearestLights(
        std::vector<Light*>* lights_vec, const Mesh* mesh, float max_light_render_dist
) {
    std::vector<int> result;

    std::vector<LightDistSt> light_dist;
    light_dist.reserve(lights_vec->size());
    for (int i = 0; i < lights_vec->size() and result.size() <= maxLightCount; ++i) {
        Light* light = (*lights_vec)[i];
        if (!light->getGlobalVisible())
            continue;
        if (light->getLightType() == Light::LightTypeEnum::DirectionalLight)
            result.push_back(i);
        else
            light_dist.push_back({mesh->getAABBDistanceToPoint(light->getGlobalPosition()), i});
    }

    // if
    // TODO: Optimize

    std::sort(light_dist.begin(), light_dist.end(), [](LightDistSt& a, LightDistSt& b) {
        return a.length < b.length;
    });
    for (int i = 0; i < light_dist.size() and result.size() <= maxLightCount; ++i) {
        if (light_dist[i].length <= max_light_render_dist)
            result.push_back(light_dist[i].index);
    }
    return result;
}

int err_count = 0;
void OpenGL15::loadTexture(Texture* texture_ptr) {
    glGenTextures(1, &texture_ptr->textureGL_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ptr->textureGL_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_ptr->texWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_ptr->texWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_ptr->texFiltering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_ptr->texMipMapFiltering);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, texture_ptr->MipMapLodBias);

// #define in GL_COMPRESSED_RGB
// #define in GL_R3_G3_B2
//#define in GL_RGB
    if (texture_ptr->texMipMapFiltering != texture_ptr->texFiltering) // MipMap enabled
        gluBuild2DMipmaps(
                GL_TEXTURE_2D, texture_ptr->textureFormat, texture_ptr->width, texture_ptr->height,
                texture_ptr->textureFormat, GL_UNSIGNED_BYTE, texture_ptr->data.data()
        );
    else // MipMad disabled
        glTexImage2D(
                GL_TEXTURE_2D, 0, texture_ptr->textureFormat, texture_ptr->width, texture_ptr->height, 0,
                texture_ptr->textureFormat, GL_UNSIGNED_BYTE, texture_ptr->data.data()
        );

    glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGL15::unloadTexture(Texture* texture_ptr) {
    glDeleteTextures(1, &texture_ptr->textureGL_ID);
    texture_ptr->textureGL_ID = 0;
}


void OpenGL15::setMaxLightRenderDist(float dist) {
    this->maxLightRenderDist = dist;
}
//============================ End 3D ============================
