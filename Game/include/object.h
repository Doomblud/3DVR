#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec2 Texture;
    glm::vec3 Normal;
};

class Object {
public:
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> textures;
    std::vector<glm::vec3> normals;
    std::vector<Vertex> vertices;

    int numVertices;

    GLuint VBO, VAO;
    GLuint textureID;

    glm::mat4 model = glm::mat4(1.0);

    static Vertex parseFaceVertex(
        const std::string& token,
        const std::vector<glm::vec3>& positions,
        const std::vector<glm::vec2>& textures,
        const std::vector<glm::vec3>& normals) {
        Vertex out{};
        out.Position = glm::vec3(0.0f);
        out.Texture = glm::vec2(0.0f);
        out.Normal = glm::vec3(0.0f, 1.0f, 0.0f);

        int pIndex = 0;
        int tIndex = 0;
        int nIndex = 0;

        const size_t firstSlash = token.find('/');
        if (firstSlash == std::string::npos) {
            pIndex = std::stoi(token);
        } else {
            const std::string pPart = token.substr(0, firstSlash);
            if (!pPart.empty()) {
                pIndex = std::stoi(pPart);
            }

            const size_t secondSlash = token.find('/', firstSlash + 1);
            if (secondSlash == std::string::npos) {
                const std::string tPart = token.substr(firstSlash + 1);
                if (!tPart.empty()) {
                    tIndex = std::stoi(tPart);
                }
            } else {
                const std::string tPart = token.substr(firstSlash + 1, secondSlash - firstSlash - 1);
                const std::string nPart = token.substr(secondSlash + 1);
                if (!tPart.empty()) {
                    tIndex = std::stoi(tPart);
                }
                if (!nPart.empty()) {
                    nIndex = std::stoi(nPart);
                }
            }
        }

        if (pIndex > 0 && static_cast<size_t>(pIndex) <= positions.size()) {
            out.Position = positions.at(static_cast<size_t>(pIndex - 1));
        }
        if (tIndex > 0 && static_cast<size_t>(tIndex) <= textures.size()) {
            out.Texture = textures.at(static_cast<size_t>(tIndex - 1));
        }
        if (nIndex > 0 && static_cast<size_t>(nIndex) <= normals.size()) {
            out.Normal = normals.at(static_cast<size_t>(nIndex - 1));
        }

        return out;
    }

    Object(const char* path) : textureID(0) {
        std::ifstream infile(path);
        if (!infile.is_open()) {
            std::cerr << "Error: Could not open file " << path << std::endl;
            numVertices = 0;
            return;
        }

        std::string line;
        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            std::string indice;
            iss >> indice;

            if (indice == "v") {
                float x, y, z;
                iss >> x >> y >> z;
                positions.push_back(glm::vec3(x, y, z));
            } else if (indice == "vn") {
                float x, y, z;
                iss >> x >> y >> z;
                normals.push_back(glm::vec3(x, y, z));
            } else if (indice == "vt") {
                float u, v;
                iss >> u >> v;
                textures.push_back(glm::vec2(u, v));
            } else if (indice == "f") {
                std::vector<std::string> faceTokens;
                std::string token;
                while (iss >> token) {
                    faceTokens.push_back(token);
                }

                if (faceTokens.size() < 3) {
                    continue;
                }

                // Triangulate polygon faces using a fan: (0, i, i+1).
                for (size_t i = 1; i + 1 < faceTokens.size(); ++i) {
                    const Vertex v0 = parseFaceVertex(faceTokens[0], positions, textures, normals);
                    const Vertex v1 = parseFaceVertex(faceTokens[i], positions, textures, normals);
                    const Vertex v2 = parseFaceVertex(faceTokens[i + 1], positions, textures, normals);
                    vertices.push_back(v0);
                    vertices.push_back(v1);
                    vertices.push_back(v2);
                }
            }
        }

        std::cout << "Loaded model with " << vertices.size() << " vertices" << std::endl;
        infile.close();
        numVertices = static_cast<int>(vertices.size());
    }

    void makeObject(Shader& /* shader */, bool /* texture */ = true) {
        float* data = new float[8 * numVertices];
        for (int i = 0; i < numVertices; i++) {
            Vertex v = vertices.at(i);
            data[i * 8] = v.Position.x;
            data[i * 8 + 1] = v.Position.y;
            data[i * 8 + 2] = v.Position.z;

            data[i * 8 + 3] = v.Texture.x;
            data[i * 8 + 4] = v.Texture.y;

            data[i * 8 + 5] = v.Normal.x;
            data[i * 8 + 6] = v.Normal.y;
            data[i * 8 + 7] = v.Normal.z;
        }

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVertices, data, GL_STATIC_DRAW);

        // Position (location 0)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0);

        // Texture coordinate (location 1)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));

        // Normal (location 2)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(5 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        delete[] data;
    }

    void setTexture(GLuint texID) {
        textureID = texID;
    }

    void draw() {
        glBindVertexArray(this->VAO);
        if (textureID != 0) {
            glBindTexture(GL_TEXTURE_2D, textureID);
        }
        glDrawArrays(GL_TRIANGLES, 0, numVertices);
    }

    ~Object() {
        if (VBO != 0) glDeleteBuffers(1, &VBO);
        if (VAO != 0) glDeleteVertexArrays(1, &VAO);
    }
};

#endif
