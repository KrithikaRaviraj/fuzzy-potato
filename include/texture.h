#ifndef TEXTURE_H
#define TEXTURE_H

/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 5: Texture Mapping
 * Team: Krithika & Akshatha
 * Developer: Krithika
 *
 * File: include/texture.h
 * Description: Texture subsystem interface for loading, binding, configuring,
 *              and managing OpenGL 2D texture resources.
 */

#include <GL/freeglut.h>

/* Compatibility define for OpenGL 1.2 edge clamping */
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

/*
 * Load an image file from disk, upload it to the GPU as an OpenGL 2D texture
 * with linear filtering and mipmaps, and return its OpenGL texture ID.
 * Returns 0 if loading failed.
 */
GLuint texture_load(const char *filepath);

/*
 * Enable 2D texturing, bind the specified texture ID, and set GL_MODULATE mode.
 * If texture_id is 0, disables 2D texturing.
 */
void texture_bind(GLuint texture_id);

/*
 * Unbind active 2D texture and disable GL_TEXTURE_2D.
 */
void texture_unbind(void);

/*
 * Delete an allocated OpenGL texture object from GPU memory.
 */
void texture_delete(GLuint texture_id);

/*
 * Initialize the global texture subsystem: loads all planetary, solar,
 * lunar, and background texture assets once at application startup.
 */
void textures_init(void);

/*
 * Cleanup and free all allocated OpenGL texture resources upon simulation exit.
 */
void textures_cleanup(void);

/*
 * Accessors for globally loaded celestial textures.
 */
GLuint texture_get_sun(void);
GLuint texture_get_planet(int index);
GLuint texture_get_moon(void);
GLuint texture_get_stars(void);

#endif /* TEXTURE_H */
