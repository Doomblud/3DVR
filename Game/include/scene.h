#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "shader.h"
#include "object.h"

#include <glm/glm.hpp>
#include <vector>
#include <memory>

struct PointLight {
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    bool enabled;
};

struct DirectionalLight {
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
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
    Shader skyboxShader;
    Shader waterShader;
    unsigned int cubeVao;
    unsigned int cubeVbo;
    unsigned int skyboxVao;
    unsigned int skyboxVbo;
    unsigned int waterVao;
    unsigned int waterVbo;
    GLuint cubemapTexture;
    GLuint whiteTexture;

    PointLight lantern;
    DirectionalLight moon;

    float moverPhase;
    std::vector<glm::mat4> staticObjects;
    
    // Models
    std::unique_ptr<Object> temple;
    GLuint templeTexture;
    std::unique_ptr<Object> ghost;
    GLuint ghostTexture;
    std::unique_ptr<Object> lanternObject;
    GLuint lanternTexture;

    void buildStaticLayout();
    void buildSkybox();
    void buildPond();
    void drawSkybox(const glm::mat4& view, const glm::mat4& projection) const;
    void drawPond(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos) const;
    void drawCube(const glm::mat4& model, const glm::vec3& color) const;
};

#endif
