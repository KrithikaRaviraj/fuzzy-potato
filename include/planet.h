#ifndef PLANET_H
#define PLANET_H

/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 4: Rotation, Revolution & Moon
 * Team: Krithika & Akshatha
 * Developer: Krithika
 *
 * File: include/planet.h
 * Description: Reusable Planet data structure, animation state, and Earth-Moon hierarchy.
 */

/*
 * Data structure representing a planet in the Solar System.
 * Encapsulates normalized visualization dimensions, orbital position,
 * axial rotation state, animation speeds, and surface color.
 */
typedef struct {
    const char *name;          /* Planet name (e.g., "Mercury", "Earth") */
    float radius;              /* Scaled visual radius */
    float orbit_distance;      /* Radial distance from the central Sun */
    float orbit_angle;         /* Current orbital revolution angle in degrees [0, 360) */
    float orbit_speed;         /* Orbital revolution speed in degrees/second */
    float rotation_angle;      /* Current axial rotation angle in degrees [0, 360) */
    float rotation_speed;      /* Axial rotation speed in degrees/second */
    float color[3];            /* RGB diffuse color components [0.0f - 1.0f] */
} Planet;

/* Total number of standard planets in the simulation */
#define PLANET_COUNT 8

/*
 * Initialize planetary data structures and initial starting angles.
 */
void planets_init(void);

/*
 * Update planetary orbital revolution and axial rotation angles
 * based on elapsed time (delta_time in seconds).
 * Wraps all angles within [0.0f, 360.0f).
 */
void planets_update(float delta_time);

/*
 * Render all 8 planets at their dynamically updated orbital positions,
 * their axial spin, Saturn's rings, and the Earth-Moon hierarchical system.
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

/*
 * Moon state accessors for verification and telemetry.
 */
float moon_get_radius(void);
float moon_get_orbit_distance(void);
float moon_get_orbit_angle(void);
float moon_get_orbit_speed(void);
float moon_get_rotation_angle(void);
float moon_get_rotation_speed(void);

void planet_get_position(int index, float *x, float *y, float *z);
#endif /* PLANET_H */
