#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "shader.h"

#include <glm/glm.hpp>
#include <vector>

struct PointLight {
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    bool enabled;
};

class Scene {
public:
    Scene();
    ~Scene();

    void initialize();
    void update(float dt);
    void render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos);

    void toggleLantern();

private:
    Shader shader;
    unsigned int cubeVao;
    unsigned int cubeVbo;

    PointLight lantern;

    float moverPhase;
    std::vector<glm::mat4> staticObjects;

    void buildStaticLayout();
    void drawCube(const glm::mat4& model, const glm::vec3& color) const;
};

#endif
