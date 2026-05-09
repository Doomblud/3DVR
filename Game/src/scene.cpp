#include "scene.h"

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
    : cubeVao(0), cubeVbo(0), moverPhase(0.0f) {
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

    // Ground
    drawCube(staticObjects[0], glm::vec3(0.22f, 0.30f, 0.20f));

    // Shrine and walls
    for (size_t i = 1; i < staticObjects.size(); ++i) {
        drawCube(staticObjects[i], glm::vec3(0.52f, 0.45f, 0.36f));
    }

    // Lantern cube
    glm::mat4 lanternModel(1.0f);
    lanternModel = glm::translate(lanternModel, lantern.position);
    lanternModel = glm::scale(lanternModel, glm::vec3(0.35f, 0.35f, 0.35f));
    drawCube(lanternModel, lantern.enabled ? glm::vec3(1.0f, 0.75f, 0.30f) : glm::vec3(0.28f, 0.24f, 0.20f));

    // Simple moving object placeholder
    glm::mat4 mover(1.0f);
    mover = glm::translate(mover, glm::vec3(2.8f * sin(moverPhase), 0.35f, 2.2f * cos(moverPhase * 0.7f)));
    mover = glm::scale(mover, glm::vec3(0.45f, 0.45f, 0.45f));
    drawCube(mover, glm::vec3(0.25f, 0.55f, 0.78f));
}

void Scene::toggleLantern() {
    lantern.enabled = !lantern.enabled;
    std::cout << "Lantern " << (lantern.enabled ? "ON" : "OFF") << '\n';
}
