#include "scene.h"
#include "texture.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>

#include <array>
#include <cmath>
#include <iostream>

namespace {
const glm::vec3 moonMarkerPosition(6.0f, 10.0f, -28.0f);
const glm::vec3 lanternBasePosition(3.7f, -0.38f, -2.35f);
constexpr float lanternModelScale = 0.12f;

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

float skyboxVertices[] = {
    -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f
};

float pondVertices[] = {
    -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
     1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
     1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,
     1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f,
    -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f
};

float smoothStep(float edge0, float edge1, float value) {
    const float t = glm::clamp((value - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

float hashStar(int x, int y, int face) {
    const float seed = static_cast<float>(x * 127 + y * 311 + face * 74);
    return glm::fract(sin(seed) * 43758.5453f);
}

glm::vec3 cubemapDirection(int face, float u, float v) {
    switch (face) {
        case 0: return glm::normalize(glm::vec3(1.0f, -v, -u));
        case 1: return glm::normalize(glm::vec3(-1.0f, -v, u));
        case 2: return glm::normalize(glm::vec3(u, 1.0f, v));
        case 3: return glm::normalize(glm::vec3(u, -1.0f, -v));
        case 4: return glm::normalize(glm::vec3(u, -v, 1.0f));
        default: return glm::normalize(glm::vec3(-u, -v, -1.0f));
    }
}

glm::vec3 nightSkyColor(const glm::vec3& direction) {
    const glm::vec3 horizonColor(10.0f, 18.0f, 36.0f);
    const glm::vec3 upperSkyColor(2.0f, 6.0f, 20.0f);
    const glm::vec3 zenithColor(0.0f, 2.0f, 10.0f);

    const float height = glm::clamp(direction.y * 0.5f + 0.5f, 0.0f, 1.0f);
    glm::vec3 color = glm::mix(horizonColor, upperSkyColor, smoothStep(0.18f, 0.72f, height));
    color = glm::mix(color, zenithColor, smoothStep(0.62f, 1.0f, height));

    const float horizonGlow = 1.0f - smoothStep(0.0f, 0.38f, std::abs(direction.y));
    color += glm::vec3(8.0f, 9.0f, 18.0f) * horizonGlow;
    return color;
}

GLuint createProceduralCubemap() {
    constexpr int faceSize = 128;
    GLuint textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    const glm::vec3 moonDirection = glm::normalize(moonMarkerPosition);
    const glm::vec3 moonRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), moonDirection));
    const glm::vec3 moonUp = glm::normalize(glm::cross(moonDirection, moonRight));
    const glm::vec3 moonColor(238.0f, 246.0f, 255.0f);

    for (int face = 0; face < 6; ++face) {
        std::array<unsigned char, faceSize * faceSize * 3> pixels{};
        for (int y = 0; y < faceSize; ++y) {
            for (int x = 0; x < faceSize; ++x) {
                const float u = 2.0f * (static_cast<float>(x) + 0.5f) / static_cast<float>(faceSize) - 1.0f;
                const float v = 2.0f * (static_cast<float>(y) + 0.5f) / static_cast<float>(faceSize) - 1.0f;
                const glm::vec3 direction = cubemapDirection(face, u, v);
                glm::vec3 color = nightSkyColor(direction);

                const float starSample = hashStar(x, y, face);
                const float starMask = direction.y > -0.05f && starSample > 0.992f ? 1.0f : 0.0f;
                const float starStrength = starMask * (25.0f + 95.0f * hashStar(x + 23, y + 19, face));

                const float moonFacing = glm::dot(direction, moonDirection);
                const glm::vec2 moonPlane(
                    glm::dot(direction, moonRight),
                    glm::dot(direction, moonUp));
                const float moonDistance = length(moonPlane) / glm::max(moonFacing, 0.001f);
                const float moonDisc = 1.0f - smoothStep(0.055f, 0.065f, moonDistance);
                const float moonHalo = (1.0f - smoothStep(0.065f, 0.22f, moonDistance)) * 0.28f;
                const int i = (y * faceSize + x) * 3;

                color = glm::mix(color, moonColor, moonDisc);
                color += moonColor * moonHalo;
                color += glm::vec3(starStrength);
                pixels[i] = static_cast<unsigned char>(glm::clamp(color.r, 0.0f, 255.0f));
                pixels[i + 1] = static_cast<unsigned char>(glm::clamp(color.g, 0.0f, 255.0f));
                pixels[i + 2] = static_cast<unsigned char>(glm::clamp(color.b, 0.0f, 255.0f));
            }
        }

        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
            0,
            GL_RGB,
            faceSize,
            faceSize,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            pixels.data());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
}

Scene::Scene()
    : cubeVao(0),
      cubeVbo(0),
      skyboxVao(0),
      skyboxVbo(0),
      waterVao(0),
      waterVbo(0),
      cubemapTexture(0),
      whiteTexture(0),
      moverPhase(0.0f),
      templeTexture(0),
      ghostTexture(0),
      lanternTexture(0) {
    moon.direction = glm::normalize(glm::vec3(-0.35f, -1.0f, -0.15f));
    moon.color = glm::vec3(0.72f, 0.80f, 1.0f);
    moon.intensity = 0.85f;

    lantern.position = lanternBasePosition + glm::vec3(0.0f, 1.08f, 0.0f);
    lantern.color = glm::vec3(1.0f, 0.8f, 0.45f);
    lantern.intensity = 1.6f;
    lantern.enabled = true;
}

Scene::~Scene() {
    if (cubemapTexture != 0) {
        glDeleteTextures(1, &cubemapTexture);
    }
    if (whiteTexture != 0) {
        glDeleteTextures(1, &whiteTexture);
    }
    if (lanternTexture != 0) {
        glDeleteTextures(1, &lanternTexture);
    }
    if (waterVbo != 0) {
        glDeleteBuffers(1, &waterVbo);
    }
    if (waterVao != 0) {
        glDeleteVertexArrays(1, &waterVao);
    }
    if (skyboxVbo != 0) {
        glDeleteBuffers(1, &skyboxVbo);
    }
    if (skyboxVao != 0) {
        glDeleteVertexArrays(1, &skyboxVao);
    }
    if (cubeVbo != 0) {
        glDeleteBuffers(1, &cubeVbo);
    }
    if (cubeVao != 0) {
        glDeleteVertexArrays(1, &cubeVao);
    }
}

void Scene::initialize() {
    shader = Shader(GAME_SHADER_DIR "/scene.vert", GAME_SHADER_DIR "/scene.frag");
    skyboxShader = Shader(GAME_SHADER_DIR "/skybox.vert", GAME_SHADER_DIR "/skybox.frag");
    waterShader = Shader(GAME_SHADER_DIR "/water.vert", GAME_SHADER_DIR "/water.frag");
    whiteTexture = TextureLoader::createWhiteTexture();
    cubemapTexture = createProceduralCubemap();

    glGenVertexArrays(1, &cubeVao);
    glGenBuffers(1, &cubeVbo);

    glBindVertexArray(cubeVao);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        6 * sizeof(float),
        reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

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

    lanternObject = std::make_unique<Object>(GAME_ASSET_DIR "/models/Lantern.obj");
    lanternObject->makeObject(shader, true);
    lanternTexture = TextureLoader::loadTexture(GAME_ASSET_DIR "/textures/Lantern_Diffuse.png");
    lanternObject->setTexture(lanternTexture);
    
    std::cout << "Temple model initialized with texture" << std::endl;

    buildStaticLayout();
    buildSkybox();
    buildPond();
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

}

void Scene::buildSkybox() {
    glGenVertexArrays(1, &skyboxVao);
    glGenBuffers(1, &skyboxVbo);

    glBindVertexArray(skyboxVao);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void Scene::buildPond() {
    glGenVertexArrays(1, &waterVao);
    glGenBuffers(1, &waterVbo);

    glBindVertexArray(waterVao);
    glBindBuffer(GL_ARRAY_BUFFER, waterVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pondVertices), pondVertices, GL_STATIC_DRAW);

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

void Scene::drawSkybox(const glm::mat4& view, const glm::mat4& projection) const {
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    skyboxShader.use();
    skyboxShader.setMat4("uView", glm::mat4(glm::mat3(view)));
    skyboxShader.setMat4("uProjection", projection);
    skyboxShader.setInt("uSkybox", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glBindVertexArray(skyboxVao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

void Scene::drawPond(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos) const {
    glm::mat4 pondModel(1.0f);
    pondModel = glm::translate(pondModel, glm::vec3(-4.0f, -0.34f, 2.4f));
    pondModel = glm::scale(pondModel, glm::vec3(3.5f, 1.0f, 2.4f));

    waterShader.use();
    waterShader.setMat4("uModel", pondModel);
    waterShader.setMat4("uView", view);
    waterShader.setMat4("uProjection", projection);
    waterShader.setVec3("uViewPos", cameraPos);
    waterShader.setFloat("uTime", moverPhase);
    waterShader.setInt("uSkybox", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glBindVertexArray(waterVao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
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
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, whiteTexture);
    drawCube(staticObjects[0], glm::vec3(0.5f, 0.5f, 0.5f));

    // Shrine and walls
    for (size_t i = 1; i < staticObjects.size(); ++i) {
        drawCube(staticObjects[i], glm::vec3(0.7f, 0.65f, 0.5f));
    }

    if (lanternObject) {
        lanternObject->model = glm::mat4(1.0f);
        lanternObject->model = glm::translate(lanternObject->model, lanternBasePosition);
        lanternObject->model = glm::rotate(lanternObject->model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lanternObject->model = glm::scale(lanternObject->model, glm::vec3(lanternModelScale));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, lanternTexture);
        shader.setMat4("uModel", lanternObject->model);
        shader.setVec3("uObjectColor", lantern.enabled ? glm::vec3(1.0f, 0.82f, 0.48f) : glm::vec3(0.24f, 0.22f, 0.20f));
        shader.setFloat("uOpacity", 1.0f);
        lanternObject->draw();
    } else {
        glm::mat4 lanternModel(1.0f);
        lanternModel = glm::translate(lanternModel, lantern.position);
        lanternModel = glm::scale(lanternModel, glm::vec3(0.35f, 0.35f, 0.35f));
        drawCube(lanternModel, lantern.enabled ? glm::vec3(1.0f, 0.75f, 0.30f) : glm::vec3(0.28f, 0.24f, 0.20f));
    }

    // Render temple model (test if it shows)
    if (temple) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, templeTexture);
        shader.setMat4("uModel", temple->model);
        shader.setVec3("uObjectColor", glm::vec3(1.0f, 1.0f, 1.0f));  // White so texture shows
        shader.setFloat("uOpacity", 1.0f);
        temple->draw();
    }

    drawPond(view, projection, cameraPos);
    shader.use();

    // Animated ghost.
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

    drawSkybox(view, projection);
}

void Scene::toggleLantern() {
    lantern.enabled = !lantern.enabled;
    std::cout << "Lantern " << (lantern.enabled ? "ON" : "OFF") << '\n';
}
