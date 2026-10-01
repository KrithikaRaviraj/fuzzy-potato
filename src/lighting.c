/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 6: Lighting & Shading
 * Developer: Krithika
 *
 * File: src/lighting.c
 * Description: Implementation of fixed-function OpenGL lighting.
 */

#include <GL/freeglut.h>
#include "lighting.h"

/*
 * Light 0 parameter configuration:
 * GL_LIGHT0 acts as the central point light source representing solar illumination.
 */
static const GLfloat light_ambient[]  = { 0.15f, 0.15f, 0.15f, 1.0f };   /* Baseline ambient space light */
static const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 0.98f, 1.0f };   /* Warm sunlight spectrum */
static const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };   /* Pure white specular highlight */
static const GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };   /* Point light at origin (w = 1.0) */

/* Global ambient light for deep-space background fill */
static const GLfloat global_ambient[] = { 0.05f, 0.05f, 0.08f, 1.0f };

void lighting_init(void) {
    /* Configure light components for GL_LIGHT0 */
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    /*
     * Configure attenuation:
     * In educational visualization, inverse-square physical attenuation (1/d^2)
     * would render outer planets (Neptune at distance 15.0) receiving ~1/36th
     * of Mercury's light, making them unreadably dark.
     * We configure constant attenuation = 1.0 so that all planets receive
     * balanced, clearly readable illumination while material differences
     * produce realistic depth.
     */
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f);

    /* Enable lighting master switch and GL_LIGHT0 source */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    /*
     * Configure Global Lighting Model:
     * 1. GL_LIGHT_MODEL_AMBIENT provides a subtle cosmic ambient base
     *    so that planetary night sides show texture outlines rather than pitch black.
     * 2. GL_LIGHT_MODEL_LOCAL_VIEWER computes specular reflection vectors from the
     *    actual eye position instead of an infinite +Z viewer, ensuring accurate
     *    specular highlights on spheres as the camera navigates around them.
     */
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    /*
     * Enable smooth Gouraud shading across polygons.
     * Interpolates vertex lighting colors across polygon faces to prevent
     * faceted polygon appearances on spheres.
     */
    glShadeModel(GL_SMOOTH);

    /*
     * Enable normal vector normalization.
     * When transformations include scaling, normal vectors are scaled non-uniformly,
     * which distorts diffuse and specular calculations. GL_NORMALIZE ensures that
     * all vertex normals are normalized to unit length before lighting calculations.
     */
    glEnable(GL_NORMALIZE);
}

void lighting_apply(void) {
    /*
     * In OpenGL, glLightfv(..., GL_POSITION, ...) transforms the given coordinates
     * by the current Modelview matrix. Therefore, calling lighting_apply() immediately
     * after camera_apply() (gluLookAt) places the light at world coordinate (0, 0, 0),
     * matching the center of the Sun.
     */
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

void lighting_get_ambient(float ambient[4]) {
    if (ambient) {
        for (int i = 0; i < 4; i++) ambient[i] = light_ambient[i];
    }
}

void lighting_get_diffuse(float diffuse[4]) {
    if (diffuse) {
        for (int i = 0; i < 4; i++) diffuse[i] = light_diffuse[i];
    }
}

void lighting_get_specular(float specular[4]) {
    if (specular) {
        for (int i = 0; i < 4; i++) specular[i] = light_specular[i];
    }
}

void lighting_get_position(float position[4]) {
    if (position) {
        for (int i = 0; i < 4; i++) position[i] = light_position[i];
    }
}

void lighting_get_global_ambient(float ambient[4]) {
    if (ambient) {
        for (int i = 0; i < 4; i++) ambient[i] = global_ambient[i];
    }
}

int lighting_is_local_viewer(void) {
    GLint local_viewer = 0;
    glGetIntegerv(GL_LIGHT_MODEL_LOCAL_VIEWER, &local_viewer);
    return (local_viewer != 0);
}

