#include "Camera.hpp"

Camera::Camera()
    : position(Vector<GLfloat, 3>(0.0f)), front(Vector<GLfloat, 3>(0.0f)), up(Vector<GLfloat, 3>(0.0f)),
    right(Vector<GLfloat, 3>(0.0f)), worldUp(Vector<GLfloat, 3>(0.0f)),
    yaw(0), pitch(0), moveSpeed(0), turnSpeed(0)
{ }

Camera::Camera(const Vector<GLfloat, 3>& startPosition, const Vector<GLfloat, 3>& startUp,
    GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
    :position(startPosition), front(Vector<GLfloat, 3>(0.0f, 0.0f, -1.0f)), up(normalize(cross_product(right, front))),
    right(normalize(cross_product(front, worldUp))), worldUp(startUp),
    yaw(startYaw), pitch(startPitch), moveSpeed(startMoveSpeed), turnSpeed(startTurnSpeed)
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
    std::cout << "Position: " << position << std::endl;
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
    float toRadian = 3.14159265f / 180.0f;

    front[0] = cos(yaw * toRadian) * cos(pitch * toRadian);
    front[1] = sin(pitch * toRadian);
    front[2] = sin(yaw * toRadian) * cos(pitch * toRadian);
    front = normalize(front);

    right = normalize(cross_product(front, worldUp));
    up = normalize(cross_product(right, front));
}
