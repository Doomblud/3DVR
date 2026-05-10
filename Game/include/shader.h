#ifndef GAME_SHADER_H
#define GAME_SHADER_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader {
public:
    GLuint ID = 0;

    Shader() = default;

    Shader(const char* vertexPath, const char* fragmentPath) {
        const std::string vertexCode = readTextFile(vertexPath);
        const std::string fragmentCode = readTextFile(fragmentPath);

        const GLuint vertex = compileShader(vertexCode, GL_VERTEX_SHADER);
        const GLuint fragment = compileShader(fragmentCode, GL_FRAGMENT_SHADER);
        ID = linkProgram(vertex, fragment);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use() const {
        glUseProgram(ID);
    }

    void setFloat(const char* name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name), value);
    }

    void setInt(const char* name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name), value);
    }

    void setVec3(const char* name, const glm::vec3& value) const {
        glUniform3f(glGetUniformLocation(ID, name), value.x, value.y, value.z);
    }

    void setMat4(const char* name, const glm::mat4& matrix) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

private:
    static std::string readTextFile(const char* path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "ERROR::SHADER::FILE_NOT_OPEN: " << path << '\n';
            return {};
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    static GLuint compileShader(const std::string& code, GLenum type) {
        const GLuint shader = glCreateShader(type);
        const char* source = code.c_str();
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[1024] = {};
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::COMPILE_FAILED: " << infoLog << '\n';
        }
        return shader;
    }

    static GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader) {
        const GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        GLint success = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[1024] = {};
            glGetProgramInfoLog(program, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER::LINK_FAILED: " << infoLog << '\n';
        }
        return program;
    }
};

#endif
