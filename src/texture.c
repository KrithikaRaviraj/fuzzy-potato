/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 5: Texture Mapping
 * Team: Krithika & Akshatha
 * Developer: Krithika
 *
 * File: src/texture.c
 * Description: Implementation of texture loading, OpenGL state management,
 *              filtering configuration, and resource cleanup.
 */

#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "texture.h"
#include "planet.h"

/* Static storage for loaded OpenGL texture IDs */
static GLuint sun_texture = 0;
static GLuint planet_textures[PLANET_COUNT] = { 0 };
static GLuint moon_texture = 0;
static GLuint stars_texture = 0;

/* Texture asset filepaths matching astronomical order */
static const char *planet_texture_files[PLANET_COUNT] = {
    "textures/mercury.jpg",
    "textures/venus.jpg",
    "textures/earth.jpg",
    "textures/mars.jpg",
    "textures/jupiter.jpg",
    "textures/saturn.jpg",
    "textures/uranus.jpg",
    "textures/neptune.jpg"
};

GLuint texture_load(const char *filepath) {
    if (!filepath) {
        fprintf(stderr, "[Texture] Error: NULL filepath provided.\n");
        return 0;
    }

    int width = 0, height = 0, channels = 0;
    /* Flip image vertically so image row 0 corresponds to OpenGL bottom texture coordinate (v = 0.0) */
    stbi_set_flip_vertically_on_load(1);
    unsigned char *image_data = stbi_load(filepath, &width, &height, &channels, 0);

    if (!image_data) {
        fprintf(stderr, "[Texture] Warning: Failed to load '%s' (%s).\n",
                filepath, stbi_failure_reason());
        return 0;
    }

    GLuint texture_id = 0;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    /*
     * Configure high-quality texture filtering and wrapping parameters:
     * - GL_REPEAT for horizontal (S) coordinate allows seamless 360-degree equatorial wrap.
     * - GL_CLAMP_TO_EDGE for vertical (T) coordinate prevents polar seam artifacts.
     * - GL_LINEAR_MIPMAP_LINEAR enables trilinear filtering for smooth minification.
     * - GL_LINEAR enables bilinear magnification for sharp close-up viewing.
     */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

    /* Build full mipmap chain using standard GLU utility */
    int status = gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height,
                                   format, GL_UNSIGNED_BYTE, image_data);
    if (status != 0) {
        fprintf(stderr, "[Texture] Warning: gluBuild2DMipmaps failed for '%s' (code %d).\n",
                filepath, status);
    }

    /* Release CPU image buffer once uploaded to GPU */
    stbi_image_free(image_data);
    glBindTexture(GL_TEXTURE_2D, 0);

    printf("[Texture] Loaded '%s' [%dx%d, %d channels] -> Texture ID %u\n",
           filepath, width, height, channels, texture_id);
    fflush(stdout);

    return texture_id;
}

void texture_bind(GLuint texture_id) {
    if (texture_id > 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    } else {
        glDisable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void texture_unbind(void) {
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture_delete(GLuint texture_id) {
    if (texture_id > 0) {
        glDeleteTextures(1, &texture_id);
    }
}

void textures_init(void) {
    printf("----------------------------------------------------\n");
    printf("[Texture] Loading Solar System celestial texture assets...\n");

    sun_texture = texture_load("textures/sun.jpg");

    for (int i = 0; i < PLANET_COUNT; i++) {
        planet_textures[i] = texture_load(planet_texture_files[i]);
    }

    moon_texture = texture_load("textures/moon.jpg");
    stars_texture = texture_load("textures/stars.jpg");

    printf("[Texture] All texture assets loaded successfully.\n");
    printf("----------------------------------------------------\n");
    fflush(stdout);
}

void textures_cleanup(void) {
    texture_delete(sun_texture);
    sun_texture = 0;

    for (int i = 0; i < PLANET_COUNT; i++) {
        texture_delete(planet_textures[i]);
        planet_textures[i] = 0;
    }

    texture_delete(moon_texture);
    moon_texture = 0;

    texture_delete(stars_texture);
    stars_texture = 0;

    printf("[Texture] All texture resources cleaned up.\n");
    fflush(stdout);
}

GLuint texture_get_sun(void) {
    return sun_texture;
}

GLuint texture_get_planet(int index) {
    if (index >= 0 && index < PLANET_COUNT) {
        return planet_textures[index];
    }
    return 0;
}

GLuint texture_get_moon(void) {
    return moon_texture;
}

GLuint texture_get_stars(void) {
    return stars_texture;
}
