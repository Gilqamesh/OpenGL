#define STD_IMAGE_IMPLEMENTATION

#ifndef CAMERA_HPP
# define CAMERA_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "math/Vector.hpp"
#include "math/Matrix.hpp"

class Camera
{
    public:
        enum class cameraModeType {
            OFF,
            FREE,
            FPS,
            TOPDOWN
        };

        Camera(const Vector<GLfloat, 3>& startPosition, const Vector<GLfloat, 3>& startUp,
            GLfloat startYaw, GLfloat startPitch,
            GLfloat startMoveSpeed, GLfloat startTurnSpeed);
        ~Camera();

        void keyControl(bool *keys, float deltaTime);
        void mouseControl(GLfloat xChange, GLfloat yChange);

        void                     setMode(cameraModeType mode)         { cameraMode = mode;   }
        cameraModeType           getMode(void)                  const { return (cameraMode); }
        const Vector<GLfloat, 3> &getPosition(void)             const { return (position);   }

        Matrix<GLfloat, 4, 4> calculateViewMatrix(void);

    private:
        Camera();

        Vector<GLfloat, 3>    position;
        Vector<GLfloat, 3>    front;
        Vector<GLfloat, 3>    up;
        Vector<GLfloat, 3>    right;
        Vector<GLfloat, 3>    worldUp;

        GLfloat         yaw;
        GLfloat         pitch;
        //GLfloat         roll;

        GLfloat         moveSpeed;
        GLfloat         turnSpeed;

        cameraModeType     cameraMode;
        Vector<GLfloat, 3> mode_Front;
        Vector<GLfloat, 3> mode_Up;
        Vector<GLfloat, 3> mode_Right;

        void update();
};

#endif
