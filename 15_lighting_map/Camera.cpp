#include <cmath>
#include "Camera.h"
#include "lookAt.h"

const glm::vec3 Camera::s_up = glm::vec3(0.0f, 1.0f, 0.0f);

Camera::Camera(CameraArgs args):
    m_position(args.position),
    m_direction(args.direction),
    m_yaw(args.yaw),
    m_pitch(args.pitch),
    m_deltaTime(args.deltaTime),
    m_type(args.type)
{
    calculateDirection();
};

void Camera::addYaw(float yaw) {
    m_yaw += yaw;
    
    calculateDirection();
}

void Camera::addPitch(float pitch) {
    m_pitch += pitch;
    
    if (m_pitch > 89.0f) {
        m_pitch = 89.0f;
    } else if (m_pitch < -89.0f) {
        m_pitch = -89.0f;
    }
    
    calculateDirection();
}

void Camera::setDeltaTime(float deltaTime) {
    m_deltaTime = deltaTime;
}

void Camera::moveForward() {
    m_position += s_moveNormalizer * m_deltaTime * m_direction;
    
    if (m_type == Type::fps) m_position.y = 0.0f;
}

void Camera::moveBackward() {
    m_position -= s_moveNormalizer * m_deltaTime * m_direction;
    
    if (m_type == Type::fps) m_position.y = 0.0f;
}

void Camera::moveRight() {
    m_position -= glm::normalize(glm::cross(s_up, m_direction)) * s_moveNormalizer * m_deltaTime;
    
    if (m_type == Type::fps) m_position.y = 0.0f;
}

void Camera::moveLeft() {
    m_position += glm::normalize(glm::cross(s_up, m_direction)) * s_moveNormalizer * m_deltaTime;
    
    if (m_type == Type::fps) m_position.y = 0.0f;
}

void Camera::moveForwardLeft() {
    glm::vec3 right = glm::normalize(glm::cross(s_up, m_direction));
    glm::vec3 up = glm::normalize(glm::cross(m_direction, right));
    glm::vec3 direction = glm::rotate(glm::mat4(1.0f), 45.0f, up) * glm::vec4(m_direction.x, m_direction.y, m_direction.z, 1.0f);
    
    m_position += s_moveNormalizer * m_deltaTime * direction;
    
    if (m_type == Type::fps) m_position.y = 0.0f;
}

void Camera::moveForwardRight() {
    glm::vec3 right = glm::normalize(glm::cross(s_up, m_direction));
    glm::vec3 up = glm::normalize(glm::cross(right, m_direction));
    glm::vec3 direction = glm::rotate(glm::mat4(1.0f), 45.0f, up) * glm::vec4(m_direction.x, m_direction.y, m_direction.z, 1.0f);
    
    m_position += s_moveNormalizer * m_deltaTime * direction;
    
    if (m_type == Type::fps) m_position.y = 0.0f;
}

void Camera::moveBackwardRight() {
    glm::vec3 right = glm::normalize(glm::cross(s_up, m_direction));
    glm::vec3 up = glm::normalize(glm::cross(m_direction, right));
    glm::vec3 direction = glm::rotate(glm::mat4(1.0f), 45.0f, up) * glm::vec4(m_direction.x, m_direction.y, m_direction.z, 1.0f);
    
    m_position -= s_moveNormalizer * m_deltaTime * direction;
    
    if (m_type == Type::fps) m_position.y = 0.0f;
}

void Camera::moveBackwardLeft() {
    glm::vec3 right = glm::normalize(glm::cross(s_up, m_direction));
    glm::vec3 up = glm::normalize(glm::cross(right, m_direction));
    glm::vec3 direction = glm::rotate(glm::mat4(1.0f), 45.0f, up) * glm::vec4(m_direction.x, m_direction.y, m_direction.z, 1.0f);
    
    m_position -= s_moveNormalizer * m_deltaTime * direction;
    
    if (m_type == Type::fps) m_position.y = 0.0f;
}

void Camera::calculateDirection() {
    m_direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_direction.y = sin(glm::radians(m_pitch));
    m_direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    
    m_direction = glm::normalize(m_direction);
}

glm::mat4 Camera::view() {
    return lookAt(m_position, m_position + m_direction, s_up);
}

glm::vec3 Camera::getPosition() {
    return m_position;
}
