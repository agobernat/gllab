
#include "camera.hpp"


Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up) : pos(pos), front(front), up(up)
{
    yaw = -90.0f;
    pitch = 0.0f;
    fov = 80.0f;
    lastX = Static::SCR_WIDTH / 2.0;
    lastY = Static::SCR_HEIGHT / 2.0;
    moveRemainingDuration = 0.;

    const int unit_size = 1;
    float n = 25.0f * unit_size;
    const auto aspect = (float)Static::SCR_WIDTH / (float)Static::SCR_HEIGHT;
    float left = -n * 0.5f, right = n * 0.5f, down = -n * 0.5f / aspect, upf = n * 0.5f / aspect;
    proj = glm::perspective(glm::radians(45.0f), (float)Static::SCR_WIDTH / (float)Static::SCR_HEIGHT, 0.1f, 10000.0f);
    //proj = glm::ortho(left, right, down, upf, 0.01f, 100.0f); // 

}


Camera::~Camera()
{
}

Camera::Camera()
{
}

const glm::mat4 Camera::view() const {

    return glm::lookAt(pos, pos + front, up);

}

const glm::mat4 Camera::projection() const
{
    return proj;
}

Camera Camera::getDefault()
{
    return Camera(glm::vec3(12.0f, 20.0f, 32.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::linearMove(glm::vec3 pos, glm::vec3 front, double duration)
{
    posGoal = pos;
    frontGoal = front;
    moveRemainingDuration = duration;
}

void Camera::tick(double dt)
{
    if (moveRemainingDuration <= 0)
    {
        return;
    }
    double ratio;
    if (dt > moveRemainingDuration)
    {
        ratio = 1.;
    }
    ratio = dt / moveRemainingDuration;
    this->pos = pos + (posGoal - pos) * static_cast<float>(ratio);
    this->front = front + (frontGoal - front) * static_cast<float>(ratio);
    moveRemainingDuration -= dt;
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
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);


}