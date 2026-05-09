#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraDirection {
    Forward,
    Backward,
    Left,
    Right,
    Up,
    Down
};

class Camera {
public:
    Camera(
        const glm::vec3& position = glm::vec3(0.0f, 1.6f, 6.0f),
        const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f,
        float pitch = 0.0f)
        : position(position),
          worldUp(up),
          yaw(yaw),
          pitch(pitch),
          movementSpeed(4.0f),
          mouseSensitivity(0.12f),
          zoom(45.0f) {
        updateVectors();
    }

    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    void processKeyboard(CameraDirection direction, float deltaTime) {
        const float velocity = movementSpeed * deltaTime;
        if (direction == CameraDirection::Forward) {
            position += front * velocity;
        }
        if (direction == CameraDirection::Backward) {
            position -= front * velocity;
        }
        if (direction == CameraDirection::Left) {
            position -= right * velocity;
        }
        if (direction == CameraDirection::Right) {
            position += right * velocity;
        }
        if (direction == CameraDirection::Up) {
            position += worldUp * velocity;
        }
        if (direction == CameraDirection::Down) {
            position -= worldUp * velocity;
        }
    }

    void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true) {
        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;

        yaw += xOffset;
        pitch += yOffset;

        if (constrainPitch) {
            if (pitch > 89.0f) {
                pitch = 89.0f;
            }
            if (pitch < -89.0f) {
                pitch = -89.0f;
            }
        }

        updateVectors();
    }

    void processMouseScroll(float yOffset) {
        zoom -= yOffset;
        if (zoom < 1.0f) {
            zoom = 1.0f;
        }
        if (zoom > 70.0f) {
            zoom = 70.0f;
        }
    }

    const glm::vec3& getPosition() const {
        return position;
    }

    float getZoom() const {
        return zoom;
    }

    void setMovementSpeed(float speed) {
        movementSpeed = speed;
    }

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    void updateVectors() {
        glm::vec3 newFront;
        newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        newFront.y = sin(glm::radians(pitch));
        newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(newFront);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};

#endif
