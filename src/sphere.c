/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 2: 3D Scene & First Objects
 * Developer: Krithika
 *
 * File: src/sphere.c
 * Description: Implementation of reusable 3D sphere renderer.
 */

#include <GL/freeglut.h>
#include "sphere.h"

/*
 * Render a solid 3D sphere.
 *
 * Uses FreeGLUT's glutSolidSphere which automatically computes
 * outward-pointing unit normal vectors for each polygon vertex.
 * These normal vectors are required for proper diffuse and specular
 * lighting calculations in the OpenGL fixed-function pipeline.
 */
void sphere_draw(float radius, int slices, int stacks) {
    if (radius <= 0.0f) {
        return;
    }
    if (slices < 3) {
        slices = 3;
    }
    if (stacks < 2) {
        stacks = 2;
    }

    glutSolidSphere((GLdouble)radius, slices, stacks);
}
