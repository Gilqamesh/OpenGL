#include "Camera.hpp"
#include "Utils.hpp"

Camera::Camera() {}

Camera::Camera(const Vector<GLfloat, 3>& startPosition, const Vector<GLfloat, 3>& startUp,
    GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
    :position(startPosition), front(Vector<GLfloat, 3>(0.0f, 0.0f, -1.0f)), up(normalize(cross_product(right, front))),
    right(normalize(cross_product(front, worldUp))), worldUp(startUp),
    yaw(startYaw), pitch(startPitch), moveSpeed(startMoveSpeed), turnSpeed(startTurnSpeed),
    cameraMode(FREE)
{
    update();
}

Camera::~Camera() {}

void    Camera::keyControl(bool *keys, float deltaTime)
{
    GLfloat velocity = moveSpeed * static_cast<GLfloat>(deltaTime);

    mode_Front = front;
    mode_Right = right;
    mode_Up = up;
    switch (cameraMode)
    {
    case FREE:
        mode_Front = front;
        mode_Right = right;
        mode_Up    = up;
        break;
    case FPS:
        mode_Front = Vector<float, 3>(front[0], 0.0f, front[2]);
        mode_Right = normalize(cross_product(mode_Front, worldUp));
        mode_Up    = Vector<float, 3>(0.0f, 1.0f, 0.0f);
        break;
    case TOPDOWN:
        mode_Front = Vector<float, 3>( 0.0f, 0.0f, 1.0f);
        mode_Right = Vector<float, 3>(-1.0f, 0.0f, 0.0f);
        mode_Up    = Vector<float, 3>( 0.0f, 1.0f, 0.0f);
        break;
    }

    if (keys[GLFW_KEY_W])
    {
        position += mode_Front * velocity;
    }
    if (keys[GLFW_KEY_S])
    {
        position -= mode_Front * velocity;
    }
    if (keys[GLFW_KEY_A])
    {
        position -= mode_Right * velocity;
    }
    if (keys[GLFW_KEY_D])
    {
        position += mode_Right * velocity;
    }
    if (keys[GLFW_KEY_SPACE])
    {
        position += mode_Up * velocity;
    }
    if (keys[GLFW_KEY_LEFT_CONTROL])
    {
        position -= mode_Up * velocity;
    }
}

void    Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
    xChange *= turnSpeed;
    yChange *= turnSpeed;

    yaw += xChange;
    pitch += yChange;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    update();
}

Matrix<GLfloat, 4, 4>   Camera::calculateViewMatrix(void)
{
    Matrix<GLfloat, 4, 4>   m(look_at(position, position + front, up));
    return (m);
}

void Camera::update(void)
{
    switch (cameraMode)
    {
    case FREE:
        front[0] = cos(Utils::radians(yaw)) * cos(Utils::radians(pitch));
        front[1] = sin(Utils::radians(pitch));
        front[2] = sin(Utils::radians(yaw)) * cos(Utils::radians(pitch));
        front = normalize(front);
        right = normalize(cross_product(front, worldUp));
        up = normalize(cross_product(right, front));
        break;
    case FPS:
        front[0] = cos(Utils::radians(yaw)) * cos(Utils::radians(pitch));
        front[1] = sin(Utils::radians(pitch));
        front[2] = sin(Utils::radians(yaw)) * cos(Utils::radians(pitch));
        front = normalize(front);
        right = normalize(cross_product(front, worldUp));
        up = normalize(cross_product(right, front));
        break;
    case TOPDOWN:
        front = Vector<float, 3>(0.0f, -1.0f, 0.0f);
        right = Vector<float, 3>(1.0f,  0.0f, 0.0f);
        up    = Vector<float, 3>(0.0f,  0.0f, 1.0f);
        break;
    }
}
