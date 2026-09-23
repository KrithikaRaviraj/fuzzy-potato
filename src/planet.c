/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 4: Rotation, Revolution & Moon
 * Team: Krithika & Akshatha
 * Developer: Krithika
 *
 * File: src/planet.c
 * Description: Implementation of dynamic Planet subsystem, planetary rotation,
 *              orbital revolution, Earth-Moon hierarchy, and Saturn ring.
 */

#include <stdio.h>
#include <math.h>
#include <GL/freeglut.h>
#include "planet.h"
#include "sphere.h"

#define PI_CONST 3.14159265358979323846f

/*
 * Planetary definitions preserving exact Week 3 starting angles, sizes,
 * and orbital distances, extended with educational revolution and rotation speeds.
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
 *
 * Orbital revolution speeds (pedagogical progression: inner faster, outer slower):
 *   Mercury (48.0 deg/s) > Venus (35.0) > Earth (25.0) > Mars (20.0) >
 *   Jupiter (12.0) > Saturn (8.5) > Uranus (5.5) > Neptune (3.5)
 */
static Planet planets[PLANET_COUNT] = {
    { "Mercury", 0.18f,  2.5f,  45.0f, 48.0f, 0.0f, 15.0f, { 0.70f, 0.70f, 0.72f } }, /* Silvery rocky grey */
    { "Venus",   0.28f,  3.6f, 110.0f, 35.0f, 0.0f, 10.0f, { 0.95f, 0.88f, 0.55f } }, /* Bright golden cream */
    { "Earth",   0.30f,  4.8f, 195.0f, 25.0f, 0.0f, 50.0f, { 0.18f, 0.58f, 0.92f } }, /* Vibrant ocean blue */
    { "Mars",    0.22f,  6.0f, 285.0f, 20.0f, 0.0f, 45.0f, { 0.88f, 0.30f, 0.15f } }, /* Rusty terracotta red */
    { "Jupiter", 0.70f,  8.2f,  65.0f, 12.0f, 0.0f, 90.0f, { 0.78f, 0.52f, 0.28f } }, /* Warm brownish-amber */
    { "Saturn",  0.58f, 10.5f, 155.0f,  8.5f, 0.0f, 80.0f, { 0.88f, 0.78f, 0.40f } }, /* Pale straw gold */
    { "Uranus",  0.42f, 12.8f, 240.0f,  5.5f, 0.0f, 60.0f, { 0.40f, 0.85f, 0.85f } }, /* Bright cyan / aquamarine */
    { "Neptune", 0.40f, 15.0f, 330.0f,  3.5f, 0.0f, 55.0f, { 0.12f, 0.25f, 0.85f } }  /* Deep cobalt azure */
};

/*
 * Moon specification: child satellite of Earth.
 * Radius: 0.08 (normalized relative to Earth 0.30)
 * Orbital distance from Earth: 0.70
 * Orbital revolution speed: 120.0 deg/s
 * Axial rotation speed: 120.0 deg/s (tidally locked model)
 * Color: neutral lunar grey
 */
static const float moon_radius = 0.08f;
static const float moon_orbit_distance = 0.70f;
static const float moon_orbit_speed = 120.0f;
static const float moon_rotation_speed = 120.0f;
static float moon_orbit_angle = 0.0f;
static float moon_rotation_angle = 0.0f;
static const GLfloat moon_color[3] = { 0.75f, 0.75f, 0.78f };

/*
 * Initial reference positions matching Week 3 startup.
 */
static const float initial_orbit_angles[PLANET_COUNT] = {
    45.0f, 110.0f, 195.0f, 285.0f, 65.0f, 155.0f, 240.0f, 330.0f
};

/*
 * Render a simple geometric ring around Saturn using concentric loops and a flat band.
 * Uses local coordinate space centered at Saturn.
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
    /* Restore clean starting angles matching Week 3 initial positions */
    for (int i = 0; i < PLANET_COUNT; i++) {
        planets[i].orbit_angle = initial_orbit_angles[i];
        planets[i].rotation_angle = 0.0f;
    }
    moon_orbit_angle = 0.0f;
    moon_rotation_angle = 0.0f;
}

void planets_update(float delta_time) {
    if (delta_time <= 0.0f) {
        return;
    }

    /* Update planetary revolution and rotation angles */
    for (int i = 0; i < PLANET_COUNT; i++) {
        planets[i].orbit_angle += planets[i].orbit_speed * delta_time;
        while (planets[i].orbit_angle >= 360.0f) {
            planets[i].orbit_angle -= 360.0f;
        }
        while (planets[i].orbit_angle < 0.0f) {
            planets[i].orbit_angle += 360.0f;
        }

        planets[i].rotation_angle += planets[i].rotation_speed * delta_time;
        while (planets[i].rotation_angle >= 360.0f) {
            planets[i].rotation_angle -= 360.0f;
        }
        while (planets[i].rotation_angle < 0.0f) {
            planets[i].rotation_angle += 360.0f;
        }
    }

    /* Update Moon orbital revolution and rotation angles */
    moon_orbit_angle += moon_orbit_speed * delta_time;
    while (moon_orbit_angle >= 360.0f) {
        moon_orbit_angle -= 360.0f;
    }
    while (moon_orbit_angle < 0.0f) {
        moon_orbit_angle += 360.0f;
    }

    moon_rotation_angle += moon_rotation_speed * delta_time;
    while (moon_rotation_angle >= 360.0f) {
        moon_rotation_angle -= 360.0f;
    }
    while (moon_rotation_angle < 0.0f) {
        moon_rotation_angle += 360.0f;
    }
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

float moon_get_radius(void) {
    return moon_radius;
}

float moon_get_orbit_distance(void) {
    return moon_orbit_distance;
}

float moon_get_orbit_angle(void) {
    return moon_orbit_angle;
}

float moon_get_orbit_speed(void) {
    return moon_orbit_speed;
}

float moon_get_rotation_angle(void) {
    return moon_rotation_angle;
}

float moon_get_rotation_speed(void) {
    return moon_rotation_speed;
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
         * 2. Orbital Revolution around the Sun:
         * Rotate around the global Y axis by the dynamically updated orbit_angle,
         * then translate outward along local X by orbit_distance.
         */
        glRotatef(p->orbit_angle, 0.0f, 1.0f, 0.0f);
        glTranslatef(p->orbit_distance, 0.0f, 0.0f);

        /*
         * 3. Saturn Rings:
         * Attached to Saturn's local orbital position, rendered before axial rotation.
         */
        if (i == 5) { /* Saturn */
            render_saturn_rings(p->radius * 1.35f, p->radius * 2.15f);
        }

        /*
         * 4. Earth-Moon Hierarchical Modeling:
         * The Moon is a child object of Earth's orbital position, but is NOT affected
         * by Earth's daily axial rotation.
         */
        if (i == 2) { /* Earth */
            glPushMatrix(); /* Moon orbital frame */

            /* Moon revolves around Earth in its own local orbit */
            glRotatef(moon_orbit_angle, 0.0f, 1.0f, 0.0f);
            glTranslatef(moon_orbit_distance, 0.0f, 0.0f);

            /* Moon axial rotation (isolated) */
            glPushMatrix();
            glRotatef(moon_rotation_angle, 0.0f, 1.0f, 0.0f);

            /* Lunar material properties under GL_LIGHT0 point light */
            GLfloat moon_amb[4]  = { moon_color[0] * 0.25f, moon_color[1] * 0.25f, moon_color[2] * 0.25f, 1.0f };
            GLfloat moon_diff[4] = { moon_color[0], moon_color[1], moon_color[2], 1.0f };
            GLfloat moon_spec[4] = { 0.10f, 0.10f, 0.10f, 1.0f };
            GLfloat moon_emiss[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

            glMaterialfv(GL_FRONT, GL_AMBIENT, moon_amb);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, moon_diff);
            glMaterialfv(GL_FRONT, GL_SPECULAR, moon_spec);
            glMaterialfv(GL_FRONT, GL_EMISSION, moon_emiss);
            glMaterialf(GL_FRONT, GL_SHININESS, 5.0f);

            sphere_draw(moon_radius, 20, 20);

            glPopMatrix(); /* Restore from Moon axial rotation */
            glPopMatrix(); /* Restore from Moon orbital frame */
        }

        /*
         * 5. Planet Axial Rotation:
         * Rotates the planet's spherical body around its own Y axis.
         * Isolated via glPushMatrix/glPopMatrix so it does not propagate
         * to the Moon or other coordinate frames.
         */
        glPushMatrix();
        glRotatef(p->rotation_angle, 0.0f, 1.0f, 0.0f);

        /*
         * 6. Configure material properties for fixed-function lighting.
         * The Sun (GL_LIGHT0 at origin) illuminates the inward-facing
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

        /* Draw 3D solid sphere */
        sphere_draw(p->radius, 32, 32);

        glPopMatrix(); /* Restore from planet axial rotation */

        glPopMatrix(); /* Restore from planet orbital frame */
    }
}
