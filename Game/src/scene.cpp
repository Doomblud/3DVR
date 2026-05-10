#include "scene.h"
#include "texture.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace {
float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f
};
}

Scene::Scene()
    : cubeVao(0), cubeVbo(0), moverPhase(0.0f), templeTexture(0), ghostTexture(0) {
    moon.direction = glm::normalize(glm::vec3(-0.35f, -1.0f, -0.15f));
    moon.color = glm::vec3(0.72f, 0.80f, 1.0f);
    moon.intensity = 0.85f;

    lantern.position = glm::vec3(0.0f, 2.2f, 0.0f);
    lantern.color = glm::vec3(1.0f, 0.8f, 0.45f);
    lantern.intensity = 1.6f;
    lantern.enabled = true;
}

Scene::~Scene() {
    if (cubeVbo != 0) {
        glDeleteBuffers(1, &cubeVbo);
    }
    if (cubeVao != 0) {
        glDeleteVertexArrays(1, &cubeVao);
    }
}

void Scene::initialize() {
    shader = Shader(GAME_SHADER_DIR "/scene.vert", GAME_SHADER_DIR "/scene.frag");

    glGenVertexArrays(1, &cubeVao);
    glGenBuffers(1, &cubeVbo);

    glBindVertexArray(cubeVao);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Load temple model
    temple = std::make_unique<Object>(GAME_ASSET_DIR "/models/Japanese_Temple.obj");
    temple->makeObject(shader, true);
    temple->model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f));
    temple->model = glm::scale(temple->model, glm::vec3(0.25f, 0.25f, 0.25f));  // Much larger
    
    // Load temple albedo texture
    templeTexture = TextureLoader::loadTexture(GAME_ASSET_DIR "/textures/Japanese_Temple_Paint2_Japanese_Shrine_Mat_AlbedoTransparency.png");
    temple->setTexture(templeTexture);

    // Load ghost model (uses material colors from shader fallback color for now).
    ghost = std::make_unique<Object>(GAME_ASSET_DIR "/models/Ghost.obj");
    ghost->makeObject(shader, true);
    ghostTexture = TextureLoader::createWhiteTexture();
    ghost->setTexture(ghostTexture);
    
    std::cout << "Temple model initialized with texture" << std::endl;

    buildStaticLayout();
}

void Scene::buildStaticLayout() {
    staticObjects.clear();

    glm::mat4 ground(1.0f);
    ground = glm::translate(ground, glm::vec3(0.0f, -0.5f, 0.0f));
    ground = glm::scale(ground, glm::vec3(24.0f, 0.25f, 24.0f));
    staticObjects.push_back(ground);

    glm::mat4 shrineBase(1.0f);
    shrineBase = glm::translate(shrineBase, glm::vec3(0.0f, 0.5f, -4.0f));
    shrineBase = glm::scale(shrineBase, glm::vec3(4.5f, 1.2f, 3.0f));
    staticObjects.push_back(shrineBase);

    glm::mat4 shrineTop(1.0f);
    shrineTop = glm::translate(shrineTop, glm::vec3(0.0f, 1.7f, -4.0f));
    shrineTop = glm::scale(shrineTop, glm::vec3(5.2f, 0.2f, 3.8f));
    staticObjects.push_back(shrineTop);

    glm::mat4 wallL(1.0f);
    wallL = glm::translate(wallL, glm::vec3(-8.5f, 0.5f, 0.0f));
    wallL = glm::scale(wallL, glm::vec3(0.35f, 1.0f, 12.0f));
    staticObjects.push_back(wallL);

    glm::mat4 wallR(1.0f);
    wallR = glm::translate(wallR, glm::vec3(8.5f, 0.5f, 0.0f));
    wallR = glm::scale(wallR, glm::vec3(0.35f, 1.0f, 12.0f));
    staticObjects.push_back(wallR);

    glm::mat4 wallBack(1.0f);
    wallBack = glm::translate(wallBack, glm::vec3(0.0f, 0.5f, -8.5f));
    wallBack = glm::scale(wallBack, glm::vec3(12.0f, 1.0f, 0.35f));
    staticObjects.push_back(wallBack);

    glm::mat4 lanternPole(1.0f);
    lanternPole = glm::translate(lanternPole, lantern.position - glm::vec3(0.0f, 0.9f, 0.0f));
    lanternPole = glm::scale(lanternPole, glm::vec3(0.12f, 1.8f, 0.12f));
    staticObjects.push_back(lanternPole);
}

void Scene::update(float dt) {
    moverPhase += dt;
}

void Scene::drawCube(const glm::mat4& model, const glm::vec3& color) const {
    shader.setMat4("uModel", model);
    shader.setVec3("uObjectColor", color);
    glBindVertexArray(cubeVao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Scene::render(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos) {
    shader.use();
    shader.setMat4("uView", view);
    shader.setMat4("uProjection", projection);
    shader.setVec3("uViewPos", cameraPos);

    shader.setVec3("uLight.position", lantern.position);
    shader.setVec3("uLight.color", lantern.color);
    shader.setFloat("uLight.intensity", lantern.enabled ? lantern.intensity : 0.0f);
    shader.setVec3("uMoon.direction", moon.direction);
    shader.setVec3("uMoon.color", moon.color);
    shader.setFloat("uMoon.intensity", moon.intensity);
    shader.setFloat("uLanternRange", 6.0f);
    shader.setInt("uUnlit", 0);
    shader.setFloat("uOpacity", 1.0f);
    shader.setInt("uTexture", 0);

    // Ground
    glBindTexture(GL_TEXTURE_2D, 0);
    drawCube(staticObjects[0], glm::vec3(0.5f, 0.5f, 0.5f));

    // Shrine and walls
    for (size_t i = 1; i < staticObjects.size(); ++i) {
        drawCube(staticObjects[i], glm::vec3(0.7f, 0.65f, 0.5f));
    }

    // Lantern cube
    glm::mat4 lanternModel(1.0f);
    lanternModel = glm::translate(lanternModel, lantern.position);
    lanternModel = glm::scale(lanternModel, glm::vec3(0.35f, 0.35f, 0.35f));
    drawCube(lanternModel, lantern.enabled ? glm::vec3(1.0f, 0.75f, 0.30f) : glm::vec3(0.28f, 0.24f, 0.20f));

    // Visible moon marker in the sky (unlit emissive object).
    glm::mat4 moonModel(1.0f);
    moonModel = glm::translate(moonModel, glm::vec3(14.0f, 18.0f, -26.0f));
    moonModel = glm::scale(moonModel, glm::vec3(1.6f, 1.6f, 1.6f));
    shader.setInt("uUnlit", 1);
    shader.setFloat("uOpacity", 1.0f);
    drawCube(moonModel, glm::vec3(0.92f, 0.96f, 1.0f));
    shader.setInt("uUnlit", 0);

    // Render temple model (test if it shows)
    if (temple) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, templeTexture);
        shader.setMat4("uModel", temple->model);
        shader.setVec3("uObjectColor", glm::vec3(1.0f, 1.0f, 1.0f));  // White so texture shows
        shader.setFloat("uOpacity", 1.0f);
        temple->draw();
    }

    // Animated ghost replacing the moving cube placeholder.
    // Drawn after opaque objects with alpha blending.
    if (ghost) {
        const glm::vec3 ghostPos = glm::vec3(
            2.8f * sin(moverPhase),
            0.55f + 0.18f * sin(moverPhase * 2.2f),
            2.2f * cos(moverPhase * 0.7f));

        ghost->model = glm::mat4(1.0f);
        ghost->model = glm::translate(ghost->model, ghostPos);
        ghost->model = glm::rotate(ghost->model, -moverPhase * 0.7f, glm::vec3(0.0f, 1.0f, 0.0f));
        ghost->model = glm::scale(ghost->model, glm::vec3(0.35f, 0.35f, 0.35f));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ghostTexture);
        shader.setMat4("uModel", ghost->model);
        shader.setVec3("uObjectColor", glm::vec3(0.86f, 0.92f, 1.0f));
        shader.setFloat("uOpacity", 0.48f);
        glDepthMask(GL_FALSE);
        ghost->draw();
        glDepthMask(GL_TRUE);
        shader.setFloat("uOpacity", 1.0f);
    }
}

void Scene::toggleLantern() {
    lantern.enabled = !lantern.enabled;
    std::cout << "Lantern " << (lantern.enabled ? "ON" : "OFF") << '\n';
}
