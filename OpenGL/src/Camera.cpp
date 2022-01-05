#include "Camera.hpp"
#include "Utils.hpp"

Camera::Camera()
    : position(Vector<GLfloat, 3>(0.0f)), front(Vector<GLfloat, 3>(0.0f)), up(Vector<GLfloat, 3>(0.0f)),
    right(Vector<GLfloat, 3>(0.0f)), worldUp(Vector<GLfloat, 3>(0.0f)),
    yaw(0), pitch(0), roll(0), moveSpeed(0), turnSpeed(0)
{ }

Camera::Camera(const Vector<GLfloat, 3>& startPosition, const Vector<GLfloat, 3>& startUp,
    GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
    :position(startPosition), front(Vector<GLfloat, 3>(0.0f, 0.0f, -1.0f)), up(normalize(cross_product(right, front))),
    right(normalize(cross_product(front, worldUp))), worldUp(startUp),
    yaw(startYaw), pitch(startPitch), roll(0), moveSpeed(startMoveSpeed), turnSpeed(startTurnSpeed),
    frontFPS(Vector<GLfloat, 3>(front[0], static_cast<GLfloat>(0.0f), front[2])), upFPS(0.0f, 1.0f, 0.0f), rightFPS(normalize(cross_product(frontFPS, worldUp)))
{
    update();
}

Camera::Camera(const Vector<GLfloat, 3>& startPosition, const Vector<GLfloat, 3>& startUp,
    GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed, GLfloat startRoll)
    :position(startPosition), front(Vector<GLfloat, 3>(0.0f, 0.0f, -1.0f)), up(normalize(cross_product(right, front))),
    right(normalize(cross_product(front, worldUp))), worldUp(startUp),
    yaw(startYaw), pitch(startPitch), roll(startRoll), moveSpeed(startMoveSpeed), turnSpeed(startTurnSpeed),
    frontFPS(Vector<GLfloat, 3>(front[0], static_cast<GLfloat>(0.0f), front[2])), upFPS(0.0f, 1.0f, 0.0f), rightFPS(normalize(cross_product(frontFPS, worldUp)))

{
    update();
}

Camera::Camera(const Camera &c): position(c.position), front(c.front), up(c.up),
right(c.right), worldUp(c.worldUp), yaw(c.yaw), pitch(c.pitch), moveSpeed(c.moveSpeed),
turnSpeed(c.turnSpeed) { }

Camera  &Camera::operator=(const Camera &c)
{
    if (this != &c)
    {
        position = c.position;
        front = c.front;
        up = c.up;
        right = c.right;
        worldUp = c.worldUp;
        yaw = c.yaw;
        pitch = c.pitch;
        moveSpeed = c.moveSpeed;
        turnSpeed = c.turnSpeed;
    }
    return (*this);
}

Camera::~Camera() { }

void    Camera::keyControl(bool *keys, float deltaTime)
{
    GLfloat velocity = moveSpeed * static_cast<GLfloat>(deltaTime);

    if (keys[GLFW_KEY_W])
    {
        position += front * velocity;
    }
    if (keys[GLFW_KEY_S])
    {
        position -= front * velocity;
    }
    if (keys[GLFW_KEY_A])
    {
        position -= right * velocity;
    }
    if (keys[GLFW_KEY_D])
    {
        position += right * velocity;
    }
    if (keys[GLFW_KEY_SPACE])
    {
        position += up * velocity;
    }
    if (keys[GLFW_KEY_LEFT_CONTROL])
    {
        position -= up * velocity;
    }
}

void    Camera::keyControlFPS(bool* keys, float deltaTime)
{
    GLfloat velocity = moveSpeed * static_cast<GLfloat>(deltaTime);

    if (keys[GLFW_KEY_W])
    {
        position += frontFPS * velocity;
    }
    if (keys[GLFW_KEY_S])
    {
        position -= frontFPS * velocity;
    }
    if (keys[GLFW_KEY_A])
    {
        position -= rightFPS * velocity;
    }
    if (keys[GLFW_KEY_D])
    {
        position += rightFPS * velocity;
    }
    if (keys[GLFW_KEY_SPACE])
    {
        position += upFPS * velocity;
    }
    if (keys[GLFW_KEY_LEFT_CONTROL])
    {
        position -= upFPS * velocity;
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

void    Camera::update(void)
{
    front[0] = cos(Utils::radians(yaw)) * cos(Utils::radians(pitch));
    front[1] = sin(Utils::radians(pitch));
    front[2] = sin(Utils::radians(yaw)) * cos(Utils::radians(pitch));
    front = normalize(front);

    right = normalize(cross_product(front, worldUp));
    up = normalize(cross_product(right, front));


    frontFPS[0] = front[0];
    frontFPS[2] = front[2];
    
    rightFPS = normalize(cross_product(frontFPS, worldUp));
}
