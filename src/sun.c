/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 2: 3D Scene & First Objects
 * Developer: Krithika
 *
 * File: src/sun.c
 * Description: Implementation of the central Sun 3D object.
 *
 * Architecture Notes:
 * - Uses the reusable sphere renderer (sphere_draw) from sphere.c.
 * - Model transformations are cleanly isolated using glPushMatrix() / glPopMatrix().
 * - Sets material emission (GL_EMISSION) so the Sun visually appears bright/self-lit.
 * - Resets emission to zero after drawing to prevent state leakage to other objects.
 * - Note: Material emission produces a visible glow on the Sun's surface, but in
 *   OpenGL fixed-function lighting, it does NOT cast light onto other objects.
 *   The scene's actual illumination is provided by GL_LIGHT0.
 */

#include <GL/freeglut.h>
#include "sun.h"
#include "sphere.h"

/* Visual scale configuration for the Sun */
static const float SUN_RADIUS = 1.2f;
static const int   SUN_SLICES = 40;
static const int   SUN_STACKS = 40;

/* Material properties for the Sun */
static const GLfloat sun_emission[] = { 1.0f, 0.75f, 0.1f, 1.0f };   /* Golden-yellow glow */
static const GLfloat sun_diffuse[]  = { 1.0f, 0.85f, 0.2f, 1.0f };   /* Warm yellow body */
static const GLfloat sun_ambient[]  = { 0.4f, 0.3f, 0.0f, 1.0f };    /* Warm ambient reflection */
static const GLfloat sun_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };    /* No specular highlight */

/* Default zero emission for clean state restoration */
static const GLfloat zero_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };

void sun_init(void) {
    /* Ready for future state expansion if needed */
}

float sun_get_radius(void) {
    return SUN_RADIUS;
}

void sun_render(void) {
    /*
     * 1. Isolate the model transformation matrix.
     * Pushing the modelview matrix guarantees that the Sun's local transformations
     * (translation, rotation, scaling) do not affect the camera view or subsequent objects.
     */
    glPushMatrix();

    /*
     * 2. Apply model transformation:
     * Translate the Sun to the central origin of the Solar System.
     */
    glTranslatef(0.0f, 0.0f, 0.0f);

    /*
     * 3. Apply material properties:
     * Configure the Sun's surface to display an emissive glow and warm color.
     */
    glMaterialfv(GL_FRONT, GL_EMISSION, sun_emission);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,  sun_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT,  sun_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, sun_specular);

    /*
     * 4. Render the 3D geometry using the reusable sphere renderer.
     */
    sphere_draw(SUN_RADIUS, SUN_SLICES, SUN_STACKS);

    /*
     * 5. Reset material emission back to zero.
     * Critical in fixed-function OpenGL state machine so that other objects
     * (e.g. planets introduced in later weeks) do not inherit this emission.
     */
    glMaterialfv(GL_FRONT, GL_EMISSION, zero_emission);

    /*
     * 6. Restore the modelview matrix.
     */
    glPopMatrix();
}
