#include <GL/freeglut.h>
#include "camera.h"
#include "planet.h"

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
static const float VIEW_DISTANCE_STEP = 1.0f;
static int focused_planet = -1;

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
                case 'z':
        case 'Z':
            camera_z -= VIEW_DISTANCE_STEP;
            break;

        case 'x':
        case 'X':
            camera_z += VIEW_DISTANCE_STEP;
            break;
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

                case '1':
            camera_focus_planet(0);
            break;

        case '2':
            camera_focus_planet(1);
            break;

        case '3':
            camera_focus_planet(2);
            break;

        case '4':
            camera_focus_planet(3);
            break;

        case '5':
            camera_focus_planet(4);
            break;

        case '6':
            camera_focus_planet(5);
            break;

        case '7':
            camera_focus_planet(6);
            break;

        case '8':
            camera_focus_planet(7);
            break;

        case '0':
            camera_clear_focus();
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

    float target_x = 0.0f;
    float target_y = 0.0f;
    float target_z = 0.0f;

    if (focused_planet >= 0)
    {
        planet_get_position(
            focused_planet,
            &target_x,
            &target_y,
            &target_z
        );

        camera_x = target_x;
        camera_y = target_y + 5.0f;
        camera_z = target_z + 8.0f;
    }

    gluLookAt(
        camera_x, camera_y, camera_z,
        target_x, target_y, target_z,
        0.0f, 1.0f, 0.0f
    );
}

/* Reset camera */
void camera_reset(void)
{
    camera_x = 0.0f;
    camera_y = 16.0f;
    camera_z = 28.0f;
}
void camera_focus_planet(int index)
{
    if (index < 0 || index >= planets_get_count())
    {
        focused_planet = -1;
        return;
    }

    focused_planet = index;

    float planet_x;
    float planet_y;
    float planet_z;

    planet_get_position(
        focused_planet,
        &planet_x,
        &planet_y,
        &planet_z
    );

    camera_x = planet_x;
    camera_y = planet_y + 5.0f;
    camera_z = planet_z + 8.0f;
}

void camera_clear_focus(void)
{
    focused_planet = -1;
    camera_reset();
}