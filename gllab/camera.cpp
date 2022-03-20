#include "camera.hpp"
#include "macros.hpp"

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up) : pos(pos), front(front), up(up)
{
    yaw = -90.0f;
    pitch = 0.0f;
    fov = 80.0f;
    lastX = SCR_WIDTH / 2.0;
    lastY = SCR_HEIGHT / 2.0;
}


Camera::~Camera()
{
}

Camera::Camera()
{
}

glm::mat4 Camera::view() {

    return glm::lookAt(pos, pos + front, up);

}

void Camera::move(float x, float y) {
    float xoffset = x - lastX;
    float yoffset = lastY - y;
    lastX = x;
    lastY = y;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.z = sin(glm::radians(pitch));
    front = glm::normalize(direction);


}