/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 3: Complete Basic Solar System
 * Team: Krithika & Akshatha
 * Developer: Krithika
 *
 * File: src/planet.c
 * Description: Implementation of reusable Planet subsystem, 8 planets, and Saturn ring.
 */

#include <stdio.h>
#include <math.h>
#include <GL/freeglut.h>
#include "planet.h"
#include "sphere.h"

#define PI_CONST 3.14159265358979323846f
#define DEG2RAD(d) ((d) * (PI_CONST / 180.0f))

/*
 * Planetary definitions in strict astronomical order from the Sun outward.
 *
 * Sizing constraint verification:
 *   Mercury (0.18)  -> Smallest planet
 *   Venus (0.28)    -> Venus ≈ Earth
 *   Earth (0.30)    -> Slightly larger than Venus
 *   Mars (0.22)     -> Mars < Earth, Mars > Mercury
 *   Jupiter (0.70)  -> Largest planet
 *   Saturn (0.58)   -> Saturn < Jupiter
 *   Uranus (0.42)   -> Smaller than Saturn
 *   Neptune (0.40)  -> Uranus ≈ Neptune
 *
 * Orbital distance constraint verification (strictly increasing):
 *   Mercury (2.5) < Venus (3.6) < Earth (4.8) < Mars (6.0) <
 *   Jupiter (8.2) < Saturn (10.5) < Uranus (12.8) < Neptune (15.0)
 */
static const Planet planets[PLANET_COUNT] = {
    { "Mercury", 0.18f,  2.5f,  45.0f, { 0.70f, 0.70f, 0.72f } }, /* Silvery rocky grey */
    { "Venus",   0.28f,  3.6f, 110.0f, { 0.95f, 0.88f, 0.55f } }, /* Bright golden cream */
    { "Earth",   0.30f,  4.8f, 195.0f, { 0.18f, 0.58f, 0.92f } }, /* Vibrant ocean blue */
    { "Mars",    0.22f,  6.0f, 285.0f, { 0.88f, 0.30f, 0.15f } }, /* Rusty terracotta red */
    { "Jupiter", 0.70f,  8.2f,  65.0f, { 0.78f, 0.52f, 0.28f } }, /* Warm brownish-amber */
    { "Saturn",  0.58f, 10.5f, 155.0f, { 0.88f, 0.78f, 0.40f } }, /* Pale straw gold */
    { "Uranus",  0.42f, 12.8f, 240.0f, { 0.40f, 0.85f, 0.85f } }, /* Bright cyan / aquamarine */
    { "Neptune", 0.40f, 15.0f, 330.0f, { 0.12f, 0.25f, 0.85f } }  /* Deep cobalt azure */
};

/*
 * Render a simple geometric ring around Saturn using concentric loops and a flat band.
 * Uses local coordinate space centered at the planet.
 */
static void render_saturn_rings(float inner_radius, float outer_radius) {
    const int segments = 64;

    glPushMatrix();

    /* Tilt Saturn's ring system (~25 degrees) relative to its orbital plane */
    glRotatef(25.0f, 1.0f, 0.0f, 0.4f);

    /* Render ring as flat geometry without lighting distortions */
    glDisable(GL_LIGHTING);
    glColor3f(0.78f, 0.72f, 0.55f); /* Warm golden ring dust */

    /* Flat ring disk using quad strip */
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; i++) {
        float angle = (float)i * (2.0f * PI_CONST / (float)segments);
        float cos_a = cosf(angle);
        float sin_a = sinf(angle);
        glVertex3f(inner_radius * cos_a, 0.0f, inner_radius * sin_a);
        glVertex3f(outer_radius * cos_a, 0.0f, outer_radius * sin_a);
    }
    glEnd();

    /* Subtle outline rings for geometric definition */
    glColor3f(0.85f, 0.80f, 0.65f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++) {
        float angle = (float)i * (2.0f * PI_CONST / (float)segments);
        glVertex3f(inner_radius * cosf(angle), 0.0f, inner_radius * sinf(angle));
    }
    glEnd();

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++) {
        float angle = (float)i * (2.0f * PI_CONST / (float)segments);
        glVertex3f(outer_radius * cosf(angle), 0.0f, outer_radius * sinf(angle));
    }
    glEnd();

    glEnable(GL_LIGHTING);
    glPopMatrix();
}

void planets_init(void) {
    /* Ready for future feature expansion */
}

int planets_get_count(void) {
    return PLANET_COUNT;
}

const Planet* planets_get(int index) {
    if (index < 0 || index >= PLANET_COUNT) {
        return NULL;
    }
    return &planets[index];
}

void planets_render(void) {
    for (int i = 0; i < PLANET_COUNT; i++) {
        const Planet *p = &planets[i];

        /*
         * 1. Isolate the transformation matrix for this planet.
         * Guarantees that this planet's translation and rotation never affect
         * the Sun, other planets, or the global camera view.
         */
        glPushMatrix();

        /*
         * 2. Hierarchical Transformation:
         * Rotate to the planet's static orbital angle around the Y axis,
         * then translate outward by its orbital distance along the local X axis.
         * Equivalent to placing the planet at (dist * cos(angle), 0, dist * sin(angle)).
         */
        glRotatef(p->orbit_angle, 0.0f, 1.0f, 0.0f);
        glTranslatef(p->orbit_distance, 0.0f, 0.0f);

        /*
         * 3. Render Saturn's ring if this is Saturn.
         * The ring is rendered in Saturn's local frame before drawing the sphere.
         */
        if (i == 5) { /* Saturn */
            render_saturn_rings(p->radius * 1.35f, p->radius * 2.15f);
        }

        /*
         * 4. Configure material properties for fixed-function lighting.
         * The Sun (GL_LIGHT0 at the origin) illuminates the inward-facing
         * hemisphere of each planet via diffuse reflection.
         */
        GLfloat mat_ambient[4]  = { p->color[0] * 0.25f, p->color[1] * 0.25f, p->color[2] * 0.25f, 1.0f };
        GLfloat mat_diffuse[4]  = { p->color[0], p->color[1], p->color[2], 1.0f };
        GLfloat mat_specular[4] = { 0.15f, 0.15f, 0.15f, 1.0f };
        GLfloat mat_emission[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; /* Non-emissive */

        glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
        glMaterialf(GL_FRONT, GL_SHININESS, 10.0f);

        /*
         * 5. Draw the 3D solid sphere using the reusable sphere module.
         */
        sphere_draw(p->radius, 32, 32);

        /*
         * 6. Restore the transformation matrix.
         */
        glPopMatrix();
    }
}
