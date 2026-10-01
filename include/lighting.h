#ifndef LIGHTING_H
#define LIGHTING_H

/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 6: Lighting & Shading
 * Developer: Krithika
 *
 * File: include/lighting.h
 * Description: Header for fixed-function OpenGL lighting system.
 *
 * Architecture Notes:
 * In fixed-function OpenGL, lighting is computed per vertex using the Phong
 * reflection model: I = I_ambient + I_diffuse + I_specular + I_emission.
 *
 * Illumination across the Solar System is provided exclusively by GL_LIGHT0,
 * positioned at the world origin (0, 0, 0) coincident with the Sun.
 * The Sun itself is rendered self-luminously with photographic texture, while
 * GL_LIGHT0 casts ambient, diffuse, and specular illumination onto all 8
 * planets and the Moon.
 */

/*
 * Initialize OpenGL lighting state.
 * Configures GL_LIGHT0 properties, global scene ambient light, local viewer
 * calculation, smooth Gouraud shading, and enables GL_NORMALIZE.
 */
void lighting_init(void);

/*
 * Position the light source in world coordinates.
 * Must be called immediately after the camera view matrix (gluLookAt) is applied
 * so that GL_LIGHT0 is anchored at world origin (0, 0, 0).
 */
void lighting_apply(void);

/*
 * Lighting state accessors for verification and unit testing.
 */
void lighting_get_ambient(float ambient[4]);
void lighting_get_diffuse(float diffuse[4]);
void lighting_get_specular(float specular[4]);
void lighting_get_position(float position[4]);
void lighting_get_global_ambient(float ambient[4]);
int lighting_is_local_viewer(void);

#endif /* LIGHTING_H */

