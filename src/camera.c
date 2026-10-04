#include <GL/freeglut.h>
#include <math.h>
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
static float camera_view_distance = 28.0f;

/* Planet focus */
static int focused_planet = -1;

/* Smooth camera transition */
static int camera_transitioning = 0;
static float transition_target_x = 0.0f;
static float transition_target_y = 0.0f;
static float transition_target_z = 0.0f;

static const float CAMERA_TRANSITION_SPEED = 0.08f;


/* Initialize camera */
void camera_init(void)
{
    camera_x = 0.0f;
    camera_y = 16.0f;
    camera_z = 28.0f;
    camera_view_distance = 28.0f;
    focused_planet = -1;
    camera_transitioning = 0;
}


/* Move camera using keyboard */
void camera_keyboard(unsigned char key)
{
    switch (key)
    {
        /* Zoom */
        case 'z':
        case 'Z':
            camera_view_distance -= VIEW_DISTANCE_STEP;

            if (camera_view_distance < 5.0f)
            {
                camera_view_distance = 5.0f;
            }

            camera_z = camera_view_distance;
            break;

        case 'x':
        case 'X':
            camera_view_distance += VIEW_DISTANCE_STEP;

            if (camera_view_distance > 50.0f)
            {
                camera_view_distance = 50.0f;
            }

            camera_z = camera_view_distance;
            break;


        /* Forward / backward */
        case 'w':
        case 'W':
            camera_z -= CAMERA_SPEED;

            if (camera_z < 5.0f)
            {
                camera_z = 5.0f;
            }

            camera_view_distance = camera_z;
            break;

        case 's':
        case 'S':
            camera_z += CAMERA_SPEED;

            if (camera_z > 50.0f)
            {
                camera_z = 50.0f;
            }

            camera_view_distance = camera_z;
            break;


        /* Left / right */
        case 'a':
        case 'A':
            camera_x -= CAMERA_SPEED;
            break;

        case 'd':
        case 'D':
            camera_x += CAMERA_SPEED;
            break;


        /* Reset */
        case 'r':
        case 'R':
            camera_reset();
            break;


        /* Planet focus */
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


        /* Clear planet focus */
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

    /*
     * If a planet is focused, use the planet as the
     * camera's viewing target.
     */
    if (focused_planet >= 0)
    {
        planet_get_position(
            focused_planet,
            &target_x,
            &target_y,
            &target_z
        );

        /*
         * Smooth transition only when a planet is first selected.
         */
        if (camera_transitioning)
        {
            float target_camera_x = target_x;
            float target_camera_y = target_y + 5.0f;
            float target_camera_z = target_z + 8.0f;

            camera_x +=
                (target_camera_x - camera_x)
                * CAMERA_TRANSITION_SPEED;

            camera_y +=
                (target_camera_y - camera_y)
                * CAMERA_TRANSITION_SPEED;

            camera_z +=
                (target_camera_z - camera_z)
                * CAMERA_TRANSITION_SPEED;

            if (fabsf(target_camera_x - camera_x) < 0.05f &&
                fabsf(target_camera_y - camera_y) < 0.05f &&
                fabsf(target_camera_z - camera_z) < 0.05f)
            {
                camera_x = target_camera_x;
                camera_y = target_camera_y;
                camera_z = target_camera_z;

                camera_transitioning = 0;
            }
        }
    }

    /*
     * Look at the focused planet or the center of
     * the Solar System.
     */
    gluLookAt(
        camera_x,
        camera_y,
        camera_z,

        target_x,
        target_y,
        target_z,

        0.0f,
        1.0f,
        0.0f
    );
}


/* Reset camera */
void camera_reset(void)
{
    focused_planet = -1;
    camera_transitioning = 0;

    camera_x = 0.0f;
    camera_y = 16.0f;
    camera_z = 28.0f;

    camera_view_distance = 28.0f;
}


/* Focus camera on a planet */
void camera_focus_planet(int index)
{
    if (index < 0 || index >= planets_get_count())
    {
        focused_planet = -1;
        camera_transitioning = 0;
        return;
    }

    focused_planet = index;
    camera_transitioning = 1;

    planet_get_position(
        focused_planet,
        &transition_target_x,
        &transition_target_y,
        &transition_target_z
    );

    transition_target_y += 5.0f;
    transition_target_z += 8.0f;
}


/* Clear planet focus */
void camera_clear_focus(void)
{
    focused_planet = -1;
    camera_transitioning = 0;

    camera_reset();
}
/* Return the currently focused planet index */
int camera_get_focused_planet(void)
{
    return focused_planet;
}