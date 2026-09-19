/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 3: Complete Basic Solar System
 * Team: Krithika & Akshatha
 * Developer: Krithika
 *
 * File: src/main.c
 * Description: 3D Scene setup, perspective projection, lighting, and integration
 *              with camera, central Sun, planetary orbits, and all 8 planets.
 *
 * 3D Coordinate System Convention (Standard OpenGL Right-Handed):
 *   +X axis : Extends to the right
 *   -X axis : Extends to the left
 *   +Y axis : Extends upwards
 *   -Y axis : Extends downwards
 *   +Z axis : Extends out of the screen (toward the viewer)
 *   -Z axis : Extends into the screen (away from the viewer)
 *   Origin (0, 0, 0) : Center of the Solar System (Sun's location)
 *
 * Graphics Transformation Pipeline:
 *   1. View Transformation  : camera_apply() via gluLookAt() aligns eye/camera space.
 *   2. Light Positioning    : lighting_apply() fixes GL_LIGHT0 at world origin (0, 0, 0).
 *   3. Solar Body Rendering : sun_render() translates and renders the central Sun.
 *   4. Orbit Paths          : orbits_render_all() draws circular line loops on X-Z plane.
 *   5. Planetary System     : planets_render() hierarchically positions and draws 8 planets.
 *   6. Projection           : gluPerspective() converts eye space to clip coordinates.
 *   7. Viewport Mapping     : glViewport() maps normalized coordinates to window pixels.
 */

#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include "camera.h"
#include "sphere.h"
#include "lighting.h"
#include "sun.h"
#include "planet.h"
#include "orbit.h"

/* Window dimensions */
static int window_width = 1024;
static int window_height = 768;

/*
 * Initialize OpenGL state for 3D rendering and lighting.
 */
static void init_opengl(void) {
    /* Set background clear color (deep space darkness) */
    glClearColor(0.02f, 0.02f, 0.05f, 1.0f);

    /* Enable depth testing to properly resolve 3D spatial occlusion */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    /* Initialize fixed-function lighting foundation */
    lighting_init();

    /* Initialize celestial bodies */
    sun_init();
    planets_init();

    /* Configure initial perspective projection matrix */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)window_width / (double)window_height, 0.1, 100.0);

    /* Return to modelview matrix */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*
 * Display callback: clears buffers, applies camera, positions light, and renders the complete scene.
 */
static void display_callback(void) {
    /* Clear color and depth buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*
     * 1. View Transformation:
     * Apply camera orientation and position using gluLookAt().
     */
    camera_apply();

    /*
     * 2. Light Source Positioning:
     * Position point light GL_LIGHT0 in world coordinates at (0, 0, 0) matching the Sun.
     */
    lighting_apply();

    /*
     * 3. Render the central Sun at (0, 0, 0).
     */
    sun_render();

    /*
     * 4. Render circular planetary orbit paths on the X-Z plane.
     */
    orbits_render_all();

    /*
     * 5. Render all 8 planets with proper scaling, materials, and positions.
     */
    planets_render();

    /* Swap front and back buffers */
    glutSwapBuffers();
}

/*
 * Reshape callback: updates viewport and perspective projection when window is resized.
 */
static void reshape_callback(int width, int height) {
    /* Prevent division by zero */
    if (height <= 0) {
        height = 1;
    }
    window_width = width;
    window_height = height;

    /* Set viewport to encompass the full window */
    glViewport(0, 0, width, height);

    /* Update projection matrix with appropriate aspect ratio */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 0.1, 100.0);

    /* Return to modelview matrix */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*
 * Keyboard callback: handles user key presses for navigation and exit.
 */
static void keyboard_callback(unsigned char key, int x, int y) {
    (void)x;
    (void)y;

    /* Exit cleanly on ESC (key code 27) or 'q' / 'Q' */
    if (key == 27 || key == 'q' || key == 'Q') {
        printf("[SolarSim] Clean exit requested by user.\n");
        fflush(stdout);
        glutLeaveMainLoop();
        return;
    }

    /* Forward navigation keys to Akshatha's camera system */
    camera_keyboard(key);

    /* Request a redraw after camera state changes */
    glutPostRedisplay();
}

/*
 * Main entry point.
 */
int main(int argc, char** argv) {
    printf("====================================================\n");
    printf(" 3D Solar System & Space Exploration Simulator\n");
    printf(" Week 3: Complete Basic Solar System\n");
    printf(" Team: Krithika & Akshatha\n");
    printf(" Developer: Krithika\n");
    printf("====================================================\n");

    /* 1. Initialize FreeGLUT */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(50, 50);

    /* 2. Create window */
    glutCreateWindow("Solar System Simulator - Week 3: Complete Basic Solar System");

    /* 3. Initialize OpenGL 3D settings and lighting */
    init_opengl();
    camera_init();

    /* 4. Register FreeGLUT callbacks */
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    glutKeyboardFunc(keyboard_callback);

    /* Allow clean return from main loop on window close */
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    printf("[SolarSim] Window created successfully.\n");
    printf("[SolarSim] Loaded %d planets: Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune.\n", planets_get_count());
    printf("[Controls] W/S: Move Forward / Backward\n");
    printf("[Controls] A/D: Move Left / Right\n");
    printf("[Controls] R:   Reset Camera Position\n");
    printf("[Controls] ESC / Q: Exit Application\n");
    fflush(stdout);

    /* 5. Enter FreeGLUT event loop */
    glutMainLoop();

    printf("[SolarSim] Application terminated cleanly.\n");
    fflush(stdout);
    return EXIT_SUCCESS;
}
