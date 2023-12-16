#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/gl.h>
#include <Old3DEngine/Render/OpenGL15.hpp>

using namespace Old3DEngine;

OpenGL15::OpenGL15(
        std::vector<Engine::SceneObject> *m,
        std::vector<Engine::SceneObject> *p,
        std::vector<Engine::SceneObject> *l
) {
    meshes = m;
    particles = p;
    lights = l;
}

void OpenGL15::render(Mesh *mesh) {
    if (mesh->getVisible()){
        std::vector<int> light_indicies = getNearestLights(mesh->getGlobalPosition());
        renderLight(light_indicies);

        glPushMatrix();
        glm::vec3 scale = mesh->getGlobalScale();
        glScalef(scale.x, scale.y, scale.z);

        glm::vec3 par_pos = mesh->parent_position;
        glTranslatef(par_pos.x, par_pos.y, par_pos.z);
        glMultMatrixf(glm::value_ptr(mesh->parent_rotation_matrix));

        glm::vec3 own_pos = mesh->position;
        glTranslatef(own_pos.x, own_pos.y, own_pos.z);
        glMultMatrixf(glm::value_ptr(mesh->rotation_matrix));
        
        // Render Textures
        if (mesh->getTexture() != nullptr) {
            glBindTexture(GL_TEXTURE_2D, mesh->getTexture()->getTextureID());
            glTexCoordPointer(2, GL_FLOAT, 0, mesh->getUV()->data());
        }

        glVertexPointer(3, GL_FLOAT, 0, mesh->getVertices()->data());
        glNormalPointer(GL_FLOAT, 0, mesh->getNormals()->data());
        //glColorPointer(3, GL_FLOAT, 0, color);
        glDrawElements(GL_TRIANGLES, mesh->getIndicies()->size(), GL_UNSIGNED_INT, mesh->getIndicies()->data());
        glBindTexture(GL_TEXTURE_2D, 0);
        glPopMatrix();
        for (int i = 0; i < light_indicies.size(); ++i)
            glDisable(GL_LIGHT0 + i);
    }
}

void OpenGL15::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (Engine::SceneObject &obj : *meshes) {
        render((Mesh*)obj.obj);
    }

    for (Engine::SceneObject &s : *particles) {
        Particles* p = (Old3DEngine::Particles*)s.obj;
        for (Particle &particle : p->particlesVector) {
            render((Mesh*)&particle);
        }
    }
}

inline void OpenGL15::renderLight(std::vector<int> indicies) {
    for (int i = 0; i < indicies.size(); ++i) {
        int index = indicies[i];
        int light_num = GL_LIGHT0 + i;
        Light *light = (Light*)(*lights)[index].obj;
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

inline std::vector<int> OpenGL15::getNearestLights(glm::vec3 position) {
    std::vector<int> result;

    std::vector<LightDistSt> light_dist;
    light_dist.reserve(lights->size());
    for (int i = 0; i < lights->size() and result.size() <= maxLightCount; ++i) {
        Light *light = (Light*)(*lights)[i].obj;
        if (!light->getVisible())
            continue;
        if (light->getLightType() == Light::LightTypeEnum::DirectionalLight)
            result.push_back(i);
        else
            light_dist.push_back({glm::length(position - light->getGlobalPosition()), i});
    }
    std::sort(light_dist.begin(), light_dist.end(), [] (LightDistSt &a, LightDistSt &b) {
        return a.length < b.length;
    });
    for (int i = 0; i < light_dist.size() and result.size() <= maxLightCount; ++i) {
        if (light_dist[i].length <= maxLightRenderDist)
            result.push_back(light_dist[i].index);
    }
    return result;
}