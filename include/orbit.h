#ifndef ORBIT_H
#define ORBIT_H

/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 3: Complete Basic Solar System
 * Team: Krithika & Akshatha
 * Developer: Krithika
 *
 * File: include/orbit.h
 * Description: Header for circular orbital path rendering.
 */

/*
 * Render a single circular orbit path on the X-Z plane centered at (0, 0, 0)
 * at the specified radial distance from the Sun.
 */
void orbit_draw(float distance);

/*
 * Render circular orbit paths for all 8 planets in the Solar System.
 */
void orbits_render_all(void);

#endif /* ORBIT_H */

