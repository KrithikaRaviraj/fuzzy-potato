/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 5: Texture Mapping
 * Developer: Krithika
 *
 * File: src/sphere.c
 * Description: Implementation of reusable 3D sphere renderer supporting
 *              both outward surface normals (for fixed-function lighting)
 *              and spherical equirectangular UV texture coordinates.
 */

#include <math.h>
#include <GL/freeglut.h>
#include "sphere.h"

#define PI_CONST 3.14159265358979323846f

/*
 * Render a solid 3D sphere centered at the local origin.
 *
 * Computes:
 * - Unit normal vectors pointing radially outward for diffuse/specular lighting.
 * - Normalized spherical UV texture coordinates (u around Y axis, v from south to north pole).
 * - 3D vertex positions scaled by radius.
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

    for (int j = 0; j < stacks; j++) {
        /* Latitude angles from south pole (-PI/2) to north pole (+PI/2) */
        float phi1 = -PI_CONST / 2.0f + (float)j * (PI_CONST / (float)stacks);
        float phi2 = -PI_CONST / 2.0f + (float)(j + 1) * (PI_CONST / (float)stacks);

        /* Vertical texture coordinate v from 0.0 (south) to 1.0 (north) */
        float v1 = (float)j / (float)stacks;
        float v2 = (float)(j + 1) / (float)stacks;

        float sin_phi1 = sinf(phi1), cos_phi1 = cosf(phi1);
        float sin_phi2 = sinf(phi2), cos_phi2 = cosf(phi2);

        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= slices; i++) {
            /* Longitude angle around the Y polar axis [0, 2*PI] */
            float theta = (float)i * (2.0f * PI_CONST / (float)slices);
            /* Horizontal texture coordinate u wrapping [1.0 -> 0.0] */
            float u = 1.0f - ((float)i / (float)slices);

            float sin_theta = sinf(theta);
            float cos_theta = cosf(theta);

            /* Vertex 1: lower stack ring at latitude phi1 */
            float nx1 = cos_phi1 * cos_theta;
            float ny1 = sin_phi1;
            float nz1 = cos_phi1 * sin_theta;
            glNormal3f(nx1, ny1, nz1);
            glTexCoord2f(u, v1);
            glVertex3f(radius * nx1, radius * ny1, radius * nz1);

            /* Vertex 2: upper stack ring at latitude phi2 */
            float nx2 = cos_phi2 * cos_theta;
            float ny2 = sin_phi2;
            float nz2 = cos_phi2 * sin_theta;
            glNormal3f(nx2, ny2, nz2);
            glTexCoord2f(u, v2);
            glVertex3f(radius * nx2, radius * ny2, radius * nz2);
        }
        glEnd();
    }
}
