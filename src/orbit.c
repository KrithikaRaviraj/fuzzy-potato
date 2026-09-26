/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 3: Complete Basic Solar System
 * Team: Krithika & Akshatha
 * Developer: Krithika
 *
 * File: src/orbit.c
 * Description: Implementation of circular orbital path rendering.
 */

#include <math.h>
#include <GL/freeglut.h>
#include "orbit.h"
#include "planet.h"

#define PI_CONST 3.14159265358979323846f

/* Number of line segments used to approximate smooth circular orbits */
#define ORBIT_SEGMENTS 100

void orbit_draw(float distance) {
    if (distance <= 0.0f) {
        return;
    }

    /*
     * Orbit lines are purely geometric visual guides and should not be
     * subjected to surface lighting or texturing.
     */
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    /* Subtle cosmic blue-grey line color */
    glColor3f(0.25f, 0.32f, 0.42f);

    /*
     * Draw circular loop on the orbital X-Z plane (y = 0):
     * x = distance * cos(theta)
     * z = distance * sin(theta)
     */
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < ORBIT_SEGMENTS; i++) {
        float angle = (float)i * (2.0f * PI_CONST / (float)ORBIT_SEGMENTS);
        glVertex3f(distance * cosf(angle), 0.0f, distance * sinf(angle));
    }
    glEnd();

    /* Restore lighting state for subsequent solid 3D geometry */
    glEnable(GL_LIGHTING);
}

void orbits_render_all(void) {
    int count = planets_get_count();
    for (int i = 0; i < count; i++) {
        const Planet *p = planets_get(i);
        if (p != NULL) {
            orbit_draw(p->orbit_distance);
        }
    }
}

