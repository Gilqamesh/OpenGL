#define STD_IMAGE_IMPLEMENTATION

#ifndef CAMERA_HPP
# define CAMERA_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "math/Vector.hpp"
#include "math/Matrix.hpp"

// ************************************************************************** //
//                              Camera Class                                  //
// ************************************************************************** //

class Camera
{
    public:
        Camera();
        Camera(const Vector<GLfloat, 3>& startPosition, const Vector<GLfloat, 3>& startUp,
            GLfloat startYaw, GLfloat startPitch,
            GLfloat startMoveSpeed, GLfloat startTurnSpeed);
        Camera(const Vector<GLfloat, 3>& startPosition, const Vector<GLfloat, 3>& startUp,
            GLfloat startYaw, GLfloat startPitch,
            GLfloat startMoveSpeed, GLfloat startTurnSpeed, GLfloat startRoll);
        Camera(const Camera &);
        Camera &operator=(const Camera &);
        ~Camera();

        void keyControl(bool *keys, float deltaTime);
        void keyControlFPS(bool* keys, float deltaTime);
        void mouseControl(GLfloat xChange, GLfloat yChange);

        Matrix<GLfloat, 4, 4> calculateViewMatrix(void);
    private:
        Vector<GLfloat, 3>    position;
        Vector<GLfloat, 3>    front;
        Vector<GLfloat, 3>    up;
        Vector<GLfloat, 3>    right;
        Vector<GLfloat, 3>    worldUp;

        GLfloat         yaw;
        GLfloat         pitch;
        GLfloat         roll;

        GLfloat         moveSpeed;
        GLfloat         turnSpeed;

        Vector<GLfloat, 3>    frontFPS;
        Vector<GLfloat, 3>    upFPS;
        Vector<GLfloat, 3>    rightFPS;

        void update();
};

#endif
