#include <GL/freeglut.h>
#include "camera.h"

/* Camera position */
static float camera_x = 0.0f;
static float camera_y = 2.0f;
static float camera_z = 5.0f;

/* Camera movement speed */
static const float CAMERA_SPEED = 0.2f;

/* Initialize camera */
void camera_init(void)
{
    camera_x = 0.0f;
    camera_y = 2.0f;
    camera_z = 5.0f;
}

/* Move camera using keyboard */
void camera_keyboard(unsigned char key)
{
    switch (key)
    {
        case 'w':
        case 'W':
            camera_z -= CAMERA_SPEED;
            break;

        case 's':
        case 'S':
            camera_z += CAMERA_SPEED;
            break;

        case 'a':
        case 'A':
            camera_x -= CAMERA_SPEED;
            break;

        case 'd':
        case 'D':
            camera_x += CAMERA_SPEED;
            break;

        case 'r':
        case 'R':
            camera_reset();
            break;

        default:
            break;
    }
}

/* Apply camera view */
void camera_apply(void)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        camera_x, camera_y, camera_z,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0
    );
}

/* Reset camera */
void camera_reset(void)
{
    camera_x = 0.0f;
    camera_y = 2.0f;
    camera_z = 5.0f;
}