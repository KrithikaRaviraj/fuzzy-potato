#ifndef SPHERE_H
#define SPHERE_H

/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 2: 3D Scene & First Objects
 * Developer: Krithika
 *
 * File: include/sphere.h
 * Description: Reusable 3D sphere rendering abstraction with normal generation.
 */

/*
 * Render a solid 3D sphere centered at the local origin (0, 0, 0)
 * with outward-facing surface normals suitable for OpenGL lighting.
 *
 * Parameters:
 *   radius - Radius of the sphere (must be > 0.0f)
 *   slices - Number of longitudinal subdivisions around the Z axis
 *   stacks - Number of latitudinal subdivisions along the Z axis
 */
void sphere_draw(float radius, int slices, int stacks);

#endif /* SPHERE_H */
