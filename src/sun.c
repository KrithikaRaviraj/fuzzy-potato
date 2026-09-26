/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 5: Texture Mapping
 * Developer: Krithika
 *
 * File: src/sun.c
 * Description: Implementation of the textured central Sun 3D object.
 *
 * Architecture Notes:
 * - Uses the reusable sphere renderer (sphere_draw) with equirectangular UV mapping.
 * - Binds the high-resolution solar photosphere texture (granulation, flares, limb darkening).
 * - Rendered self-luminously so that the photosphere details appear vibrant and unattenuated.
 * - Features slow axial rotation to realistically simulate the differential solar rotation.
 * - Cleanly restores OpenGL lighting state and unbinds textures after rendering.
 * - Point light GL_LIGHT0 remains anchored at (0, 0, 0) to illuminate the surrounding planets.
 */

#include <GL/freeglut.h>
#include "sun.h"
#include "sphere.h"
#include "texture.h"

/* Visual scale configuration for the Sun */
static const float SUN_RADIUS = 1.2f;
static const int   SUN_SLICES = 48;
static const int   SUN_STACKS = 48;

/* Solar axial rotation state */
static float sun_rotation_angle = 0.0f;
static const float sun_rotation_speed = 8.0f; /* deg/s (pedagogical slow drift) */

void sun_init(void) {
    sun_rotation_angle = 0.0f;
}

void sun_update(float delta_time) {
    if (delta_time <= 0.0f) {
        return;
    }

    sun_rotation_angle += sun_rotation_speed * delta_time;
    while (sun_rotation_angle >= 360.0f) {
        sun_rotation_angle -= 360.0f;
    }
    while (sun_rotation_angle < 0.0f) {
        sun_rotation_angle += 360.0f;
    }
}

float sun_get_radius(void) {
    return SUN_RADIUS;
}

float sun_get_rotation_angle(void) {
    return sun_rotation_angle;
}

void sun_render(void) {
    /*
     * 1. Isolate the model transformation matrix.
     */
    glPushMatrix();

    /*
     * 2. Apply model transformation:
     * Position at the central origin and apply axial rotation.
     */
    glTranslatef(0.0f, 0.0f, 0.0f);
    glRotatef(sun_rotation_angle, 0.0f, 1.0f, 0.0f);

    /*
     * 3. Render Sun self-luminously with texture:
     * Disabling lighting ensures that the solar photosphere texture is displayed
     * at full luminous brilliance without self-shadowing, while GL_LIGHT0 at
     * the origin continues to illuminate all surrounding planets.
     */
    glDisable(GL_LIGHTING);
    texture_bind(texture_get_sun());
    glColor3f(1.0f, 1.0f, 1.0f);

    /*
     * 4. Draw the textured 3D sphere.
     */
    sphere_draw(SUN_RADIUS, SUN_SLICES, SUN_STACKS);

    /*
     * 5. Clean state restoration:
     * Unbind texture and restore lighting for planetary rendering.
     */
    texture_unbind();
    glEnable(GL_LIGHTING);

    /*
     * 6. Restore the modelview matrix.
     */
    glPopMatrix();
}
