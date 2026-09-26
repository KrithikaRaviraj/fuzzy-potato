/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 5: Texture Mapping
 * Team: Krithika & Akshatha
 * Developer: Krithika
 *
 * File: src/main.c
 * Description: 3D Scene setup, perspective projection, lighting, camera integration,
 *              time-based dynamic animation loop, and 2D texture mapping.
 *
 * 3D Coordinate System Convention (Standard OpenGL Right-Handed):
 *   +X axis : Extends to the right
 *   -X axis : Extends to the left
 *   +Y axis : Extends upwards (perpendicular to the orbital plane)
 *   -Y axis : Extends downwards
 *   +Z axis : Extends out of the screen (toward the viewer)
 *   -Z axis : Extends into the screen (away from the viewer)
 *   Origin (0, 0, 0) : Center of the Solar System (Sun's location)
 *
 * Graphics Transformation Pipeline:
 *   1. View Transformation  : camera_apply() via gluLookAt() aligns eye/camera space.
 *   2. Background Stars     : Render textured celestial starfield sphere.
 *   3. Light Positioning    : lighting_apply() fixes GL_LIGHT0 at world origin (0, 0, 0).
 *   4. Solar Body Rendering : sun_render() translates, rotates, and renders textured Sun.
 *   5. Orbit Paths          : orbits_render_all() draws circular line loops on X-Z plane.
 *   6. Dynamic Planets      : planets_render() hierarchically updates and draws 8 textured planets
 *                             and the Earth-Moon child system.
 *   7. Projection           : gluPerspective() converts eye space to clip coordinates.
 *   8. Viewport Mapping     : glViewport() maps normalized coordinates to window pixels.
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
#include "texture.h"

/* Window dimensions */
static int window_width = 1024;
static int window_height = 768;

/* Previous timestamp for elapsed delta-time calculation (milliseconds) */
static int previous_time = 0;

/*
 * Render cosmic starfield background sphere.
 */
static void render_stars_background(void) {
    GLuint stars_tex = texture_get_stars();
    if (stars_tex == 0) {
        return;
    }

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);

    texture_bind(stars_tex);
    glColor3f(0.85f, 0.85f, 0.85f); /* Subtle, natural cosmic starry luminance */

    glPushMatrix();
    glRotatef(20.0f, 1.0f, 0.0f, 0.5f);
    sphere_draw(70.0f, 32, 24); /* Enclosing celestial background sphere */
    glPopMatrix();

    texture_unbind();
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

/*
 * Timer callback: computes elapsed delta time and drives planetary animation.
 */
static void timer_callback(int value) {
    (void)value;

    int current_time = glutGet(GLUT_ELAPSED_TIME);

    /* Safe first-frame initialization to prevent abnormal delta jumps */
    if (previous_time == 0) {
        previous_time = current_time;
    }

    float delta_time = (float)(current_time - previous_time) / 1000.0f;
    previous_time = current_time;

    /* Clamp delta_time to 0.1s maximum to prevent large jumps after pauses or window moves */
    if (delta_time > 0.1f) {
        delta_time = 0.1f;
    } else if (delta_time < 0.0f) {
        delta_time = 0.0f;
    }

    /* Update dynamic solar rotation, planetary rotation, and orbital revolution angles */
    sun_update(delta_time);
    planets_update(delta_time);

    /* Request screen redraw */
    glutPostRedisplay();

    /* Re-register timer for ~60 FPS (16 ms) */
    glutTimerFunc(16, timer_callback, 0);
}

/*
 * Initialize OpenGL state for 3D rendering, lighting, and textures.
 */
static void init_opengl(void) {
    /* Set background clear color (deep space darkness) */
    glClearColor(0.02f, 0.02f, 0.05f, 1.0f);

    /* Enable depth testing to properly resolve 3D spatial occlusion */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    /* Initialize fixed-function lighting foundation */
    lighting_init();

    /* Load texture assets */
    textures_init();

    /* Initialize celestial bodies and starting angles */
    sun_init();
    planets_init();

    /* Configure initial perspective projection matrix */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)window_width / (double)window_height, 0.1, 150.0);

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
     * 2. Background Starfield:
     * Render cosmic starfield sphere behind all scene geometry.
     */
    render_stars_background();

    /*
     * 3. Light Source Positioning:
     * Position point light GL_LIGHT0 in world coordinates at (0, 0, 0) matching the Sun.
     */
    lighting_apply();

    /*
     * 4. Render the central textured Sun at (0, 0, 0).
     */
    sun_render();

    /*
     * 5. Render circular planetary orbit paths on the X-Z plane.
     */
    orbits_render_all();

    /*
     * 6. Render all 8 textured planets and Earth-Moon hierarchy at dynamic positions.
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
    gluPerspective(45.0, (double)width / (double)height, 0.1, 150.0);

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

    /* Forward navigation keys to Akshatha's camera system (W/S/A/D, Z/X, R) */
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
    printf(" Week 5: Texture Mapping\n");
    printf(" Team: Krithika & Akshatha\n");
    printf(" Developer: Krithika\n");
    printf("====================================================\n");

    /* 1. Initialize FreeGLUT */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(50, 50);

    /* 2. Create window */
    glutCreateWindow("Solar System Simulator - Week 5: Texture Mapping");

    /* 3. Initialize OpenGL 3D settings, lighting, and textures */
    init_opengl();
    camera_init();

    /* 4. Register FreeGLUT callbacks */
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    glutKeyboardFunc(keyboard_callback);

    /* Initialize timer for animation */
    previous_time = glutGet(GLUT_ELAPSED_TIME);
    glutTimerFunc(16, timer_callback, 0);

    /* Allow clean return from main loop on window close */
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    printf("[SolarSim] Window created successfully.\n");
    printf("[SolarSim] Loaded %d planets + Earth Moon system + textures.\n", planets_get_count());
    printf("[Controls] W/S: Move Forward / Backward\n");
    printf("[Controls] A/D: Move Left / Right\n");
    printf("[Controls] Z/X: Zoom In / Out (Viewing Distance)\n");
    printf("[Controls] 1-8: Focus on Mercury to Neptune\n");
    printf("[Controls] 0:   Clear Planet Focus\n"); 
    printf("[Controls] R:   Reset Camera Position\n");
    printf("[Controls] ESC / Q: Exit Application\n");
    fflush(stdout);

    /* 5. Enter FreeGLUT event loop */
    glutMainLoop();

    /* 6. Release texture resources on clean exit */
    textures_cleanup();

    printf("[SolarSim] Application terminated cleanly.\n");
    fflush(stdout);
    return EXIT_SUCCESS;
}
