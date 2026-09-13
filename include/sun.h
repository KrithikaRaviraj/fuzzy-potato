#ifndef SUN_H
#define SUN_H

/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 2: 3D Scene & First Objects
 * Developer: Krithika
 *
 * File: include/sun.h
 * Description: Header for the Sun rendering module.
 */

/*
 * Initialize Sun properties.
 */
void sun_init(void);

/*
 * Render the Sun as a 3D solid sphere at the origin (0, 0, 0).
 * Demonstrates proper model transformation isolation (glPushMatrix / glPopMatrix),
 * setting material emission for visual self-illumination, and drawing via
 * the reusable sphere module.
 */
void sun_render(void);

/*
 * Get the radius of the Sun.
 */
float sun_get_radius(void);

#endif /* SUN_H */
