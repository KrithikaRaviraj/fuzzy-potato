#ifndef PLANET_H
#define PLANET_H

/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 3: Complete Basic Solar System
 * Team: Krithika & Akshatha
 * Developer: Krithika
 *
 * File: include/planet.h
 * Description: Reusable Planet data structure and rendering subsystem.
 */

/*
 * Data structure representing a planet in the Solar System.
 * Encapsulates normalized visualization dimensions, orbital position,
 * and surface color without duplicating rendering functions.
 */
typedef struct {
    const char *name;          /* Planet name (e.g., "Mercury", "Earth") */
    float radius;              /* Scaled visual radius */
    float orbit_distance;      /* Radial distance from the central Sun */
    float orbit_angle;         /* Static orbital angle around Sun in degrees */
    float color[3];            /* RGB diffuse color components [0.0f - 1.0f] */
} Planet;

/* Total number of standard planets in the simulation */
#define PLANET_COUNT 8

/*
 * Initialize planetary data structures.
 */
void planets_init(void);

/*
 * Render all 8 planets at their respective orbital positions using
 * isolated hierarchical model transformations (glPushMatrix / glPopMatrix)
 * and proper material properties under GL_LIGHT0 illumination.
 */
void planets_render(void);

/*
 * Return the total number of planets.
 */
int planets_get_count(void);

/*
 * Retrieve a read-only pointer to a planet by index (0 to PLANET_COUNT - 1).
 * Returns NULL if index is out of range.
 */
const Planet* planets_get(int index);

#endif /* PLANET_H */
