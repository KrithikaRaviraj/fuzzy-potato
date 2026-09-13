/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 2: 3D Scene & First Objects
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
static const GLfloat light_ambient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };   /* Baseline ambient space light */
static const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 0.95f, 1.0f };  /* Warm sunlight */
static const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };   /* White specular highlight */
static const GLfloat light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };   /* Point light at origin (w = 1.0) */

void lighting_init(void) {
    /* Configure light components */
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    /* Enable lighting master switch and GL_LIGHT0 source */
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

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
