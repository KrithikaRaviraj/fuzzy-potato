#include <GL/freeglut.h>
#include "camera.h"

/*
 * Camera position:
 * Configured for Week 3 to provide an elevated oblique view framing the entire
 * Solar System (from the central Sun out to Neptune and its orbit at radius 15.0).
 * Visually verified at (0.0f, 16.0f, 28.0f) to encompass all 8 planetary orbits.
 */
static float camera_x = 0.0f;
static float camera_y = 16.0f;
static float camera_z = 28.0f;

/* Camera movement speed scaled for solar system dimensions */
static const float CAMERA_SPEED = 0.5f;

/* Initialize camera */
void camera_init(void)
{
    camera_x = 0.0f;
    camera_y = 16.0f;
    camera_z = 28.0f;
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
    camera_y = 16.0f;
    camera_z = 28.0f;
}