#ifndef SUN_H
#define SUN_H

/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 5: Texture Mapping
 * Developer: Krithika
 *
 * File: include/sun.h
 * Description: Header for the Sun rendering and animation module.
 */

/*
 * Initialize Sun properties.
 */
void sun_init(void);

/*
 * Update Sun axial rotation based on elapsed delta time in seconds.
 */
void sun_update(float delta_time);

/*
 * Render the Sun as a textured 3D solid sphere at the origin (0, 0, 0).
 * Demonstrates proper model transformation isolation (glPushMatrix / glPopMatrix),
 * binding the solar photosphere texture, and rendering self-luminously.
 */
void sun_render(void);

/*
 * Get the radius of the Sun.
 */
float sun_get_radius(void);

/*
 * Get the current axial rotation angle of the Sun.
 */
float sun_get_rotation_angle(void);

#endif /* SUN_H */
