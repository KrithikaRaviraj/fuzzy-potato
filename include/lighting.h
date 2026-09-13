#ifndef LIGHTING_H
#define LIGHTING_H

/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 2: 3D Scene & First Objects
 * Developer: Krithika
 *
 * File: include/lighting.h
 * Description: Header for fixed-function OpenGL lighting system.
 *
 * Note on Lighting Architecture:
 * The Sun's visual glow is achieved using material emission (GL_EMISSION).
 * In OpenGL's fixed-function pipeline, material emission DOES NOT cast light
 * onto surrounding objects. The actual illumination across the Solar System
 * is provided exclusively by GL_LIGHT0, positioned at the center of the scene.
 */

/*
 * Initialize OpenGL lighting state.
 * Configures GL_LIGHT0 properties and enables GL_NORMALIZE.
 */
void lighting_init(void);

/*
 * Position the light source in world coordinates.
 * Must be called after the camera view matrix (gluLookAt) is applied
 * so that GL_LIGHT0 is anchored at world origin (0, 0, 0).
 */
void lighting_apply(void);

#endif /* LIGHTING_H */
