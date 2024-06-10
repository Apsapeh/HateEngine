#include "HateEngine/Resources/Level.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/gl.h>
#include <HateEngine/Render/OpenGL15.hpp>

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
    
    GLfloat mat_specular[]={1.0,1.0,1.0,1.0};
    GLfloat mat_shininess[]={50.0};
    GLfloat light_position[]={1.0,1.0,1.0,1.0};
    GLfloat white_light[]={1.0,1.0,1.0,1.0};
    glClearColor(0.0,1.0,0.0,0.0);
    //glShadeModel(GL_SMOOTH);
    //glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    //glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
    GLfloat light_ambient[]={1.0,1.0,1.0,1.0};
    /*glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
    glLightfv(GL_LIGHT0,GL_POSITION,light_position);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.2);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.5);*/
    //glLightfv(GL_LIGHT0,GL_DIFFUSE,white_light);
    //glLightfv(GL_LIGHT0,GL_SPECULAR,white_light);
    //auto a = GL_CLAMP_TO_EDGE

    //glEnableClientState(GL_COLOR_ARRAY);

    glEnable(GL_BLEND);
    //glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);


    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    //glDisable(GL_FOG);

    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    initNuklearUI();
}


//=======================> 3D <========================

static uint32_t fog_modes[] = {
    GL_LINEAR, GL_EXP, GL_EXP2
};

void OpenGL15::Render() {
    HateEngine::Level* level = this->engine->getLevel();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(
        level->settings.background_color[0], level->settings.background_color[1],
        level->settings.background_color[2], level->settings.background_color[3]
    );
    
    // Fog
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE,    fog_modes[level->settings.fog_mode]);
    glFogf(GL_FOG_DENSITY, level->settings.fog_density);
    glFogf(GL_FOG_START,   level->settings.fog_start);
    glFogf(GL_FOG_END,     level->settings.fog_end);
    glFogfv(GL_FOG_COLOR,  level->settings.fog_color);

    glPushMatrix();
    this->Draw3D(
        level->camera,
        &level->meshes,
        &level->particles,
        &level->lights
    );
    glPopMatrix();

    glDisable(GL_FOG);
    this->DrawNuklearUI(&level->ui_widgets);
}

void OpenGL15::Draw3D(
        Camera* camera,
        std::vector<Mesh*>* meshes,
        std::vector<Particles*>* particles,
        std::vector<Light*>* lights
) {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    if (camera != nullptr) {
        renderCamera(camera);
        if (camera->isSkyBoxEnabled()) {
            std::vector<Light*> n;
            render(camera->getSkyBox(), &n);
        }
    }
    glEnable(GL_LIGHTING);
    glDisable(GL_LIGHT0);

    for (const auto obj : *meshes)
        render(obj, lights);

    for (const auto* s : *particles) {
        for (const auto &particle : s->particlesVector)
            render((const Mesh*)&particle, lights);
    }


    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
}

void OpenGL15::render(const Mesh *mesh, std::vector<Light*>* lights_vec) {
    if (mesh->getVisible()) {
        glPushMatrix();
        
        std::vector<int> light_indicies;
        if (mesh->isLightShading()) {
            float max_light_render_dist = mesh->getCustomMaxLightDist();
            if (max_light_render_dist == 0)
                max_light_render_dist = this->maxLightRenderDist;
            light_indicies = getNearestLights(
                lights_vec, mesh->getGlobalPosition(),
                max_light_render_dist
            );
            renderLight(lights_vec, light_indicies);
        }
        else {
            glDisable(GL_LIGHTING);
        }
        
        
        glm::vec3 par_pos = mesh->getGlobalPosition();
        glTranslatef(par_pos.x, par_pos.y, par_pos.z);
        //std::cout << "Render pos: " << par_pos.x << " | " << par_pos.y << " | " << par_pos.z << "\n";
        glMultMatrixf(glm::value_ptr(mesh->getGlobalRotationMatrix()));

        glm::vec3 scale = mesh->getGlobalScale();
        glScalef(scale.x, scale.y, scale.z);
        
        // Render Textures
        if (mesh->getTexture() != nullptr) {
            if (not mesh->getTexture()->is_loaded)
                mesh->getTexture()->Load(loadTexture, unloadTexture);

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, mesh->getTexture()->getTextureID());
            glTexCoordPointer(2, GL_FLOAT, 0, mesh->getUV()->data());
        }

        glVertexPointer(3, GL_FLOAT, 0, mesh->getVertices()->data());
        glNormalPointer(GL_FLOAT, 0, mesh->getNormals()->data());
        //glColorPointer(3, GL_FLOAT, 0, color);
        glDrawElements(GL_TRIANGLES, mesh->getIndicies()->size(), GL_UNSIGNED_INT, mesh->getIndicies()->data());

        if (mesh->getTexture() != nullptr) {
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_TEXTURE_2D);
        }

        glPopMatrix();
        for (int i = 0; i < light_indicies.size(); ++i)
            glDisable(GL_LIGHT0 + i);
            
        if (mesh->isLightShading())
            glEnable(GL_LIGHTING);
    }
}


inline void OpenGL15::renderCamera(Camera* camera) {
    glm::mat4 Mp = glm::perspective(
        glm::radians(camera->getFOV()),
        this->engine->getAspectRatio(),
        0.1f,
        camera->getRenderDist()
    );

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(Mp));

    glm::mat4 mat = camera->getGlobalRotationMatrix();
    mat = glm::translate(mat, -camera->getGlobalPosition()) ;

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(mat));
}


inline void OpenGL15::renderLight(
    std::vector<Light*>* lights_vec, const std::vector<int>& indicies
) {
    //std::cout << "Lights: " << lights_vec->size() << "\n";
    for (int i = 0; i < indicies.size(); ++i) {
        int index = indicies[i];
        int light_num = GL_LIGHT0 + i;
        Light *light = (*lights_vec)[index];
        glm::vec3 pos = light->getGlobalPosition();
        float l_position[4] = {pos.x, pos.y, pos.z, 1.0};

        if (light->getLightType() == Light::LightTypeEnum::DirectionalLight)
            l_position[3] = 0.0;
            

        glEnable(light_num);
        glLightfv(light_num,GL_DIFFUSE,light->getColor().data());
        glLightfv(light_num,GL_POSITION, l_position);
        glLightf(light_num, GL_CONSTANT_ATTENUATION, 0.2);
        glLightf(light_num, GL_LINEAR_ATTENUATION, 0.2);
        glLightf(light_num, GL_QUADRATIC_ATTENUATION, 0.5);
    }
}

struct LightDistSt {
    float length;
    int   index;
};

inline std::vector<int> OpenGL15::getNearestLights(
    std::vector<Light*>* lights_vec, glm::vec3 position,
    float max_light_render_dist
) {    
    std::vector<int> result;

    std::vector<LightDistSt> light_dist;
    light_dist.reserve(lights_vec->size());
    for (int i = 0; i < lights_vec->size() and result.size() <= maxLightCount; ++i) {
        Light *light = (*lights_vec)[i];
        if (!light->getVisible())
            continue;
        if (light->getLightType() == Light::LightTypeEnum::DirectionalLight)
            result.push_back(i);
        else
            light_dist.push_back({glm::length(position - light->getGlobalPosition()), i});
    }
    
    //if
   //TODO: Optimize 
    
    std::sort(light_dist.begin(), light_dist.end(), [] (LightDistSt &a, LightDistSt &b) {
        return a.length < b.length;
    });
    for (int i = 0; i < light_dist.size() and result.size() <= maxLightCount; ++i) {
        if (light_dist[i].length <= max_light_render_dist)
            result.push_back(light_dist[i].index);
    }
    return result;
}



void OpenGL15::loadTexture(Texture* texture_ptr) {
    glGenTextures(1, &texture_ptr->textureGL_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ptr->textureGL_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_ptr->texWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_ptr->texWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, texture_ptr->texWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_ptr->texFiltering);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_ptr->texMipMapFiltering);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, texture_ptr->MipMapLodBias);

    if (texture_ptr->texMipMapFiltering != texture_ptr->texFiltering) // MipMap enabled
        gluBuild2DMipmaps(
            GL_TEXTURE_2D, texture_ptr->textureFormat,
            texture_ptr->width, texture_ptr->height,
            texture_ptr->textureFormat, GL_UNSIGNED_BYTE,
            texture_ptr->data.data()
        );
    else // MipMad disabled
        glTexImage2D(
            GL_TEXTURE_2D, 0, texture_ptr->textureFormat,
            texture_ptr->width, texture_ptr->height,
            0, texture_ptr->textureFormat,
            GL_UNSIGNED_BYTE, texture_ptr->data.data()
        );


    glBindTexture(GL_TEXTURE_2D, 0);
}


void OpenGL15::unloadTexture(Texture* texture_ptr) {
    glDeleteTextures(1, &texture_ptr->textureGL_ID);
    texture_ptr->textureGL_ID = 0;
}

//============================ End 3D ============================
