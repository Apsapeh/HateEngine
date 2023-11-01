#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/gl.h>
#include <Old3DEngine/Render/OpenGL15.hpp>

using namespace Old3DEngine;

GLfloat color[] = {
        0.7608, 0.9333, 0.051, 0.6549, 0.6235, 0.1647, 0.7255, 0.0157, 0.8471, 0.5098, 0.8314, 0.8627, 0.4078, 0.3216, 0.8863, 0.9098, 0.7294, 0.1804, 0.8314, 0.451, 0.5412, 0.5647, 0.4235, 0.4196
};


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
        glm::vec3 pos = mesh->getPosition();

        std::vector<int> light_indicies = getNearestLights(pos);
        renderLight(light_indicies);

        glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);

        glMultMatrixf(glm::value_ptr(mesh->getRotationMatrix()));

        // Render Textures
        const std::vector<Mesh::TextureObject>* textures = mesh->getTextures();
        for (const Mesh::TextureObject &tex : *textures) {
            glBindTexture(GL_TEXTURE_2D, tex.texture->getTextureID());
            glTexCoordPointer(2, GL_FLOAT, 0, tex.UV.data());
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
        glm::vec3 pos = light->getPosition();
        float l_position[4] = {pos.x, pos.y, pos.z, 1.0};

        if (light->getType() == Light::LightTypeEnum::DirectionalLight)
            l_position[3] = 0.0;

        glEnable(light_num);
        glLightfv(light_num,GL_DIFFUSE,light->getColor().data());
        glLightfv(light_num,GL_POSITION, l_position);
        glLightf(light_num, GL_CONSTANT_ATTENUATION, 1.0);
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
        if (light->getType() == Light::LightTypeEnum::DirectionalLight)
            result.push_back(i);
        else
            light_dist.push_back({glm::length(position - light->getPosition()), i});
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


/*void OpenGL15::eraseObjects() {
    std::lock_guard<std::mutex> guard(meshesVecMutex);
    for (OGLObject &mesh : meshes) {
        if (not mesh.is_ref)
            delete mesh.mesh;
    }
    meshes.clear();
}*/


