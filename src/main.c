/*
 * Interactive 3D Solar System and Space Exploration Simulator
 * Week 1: Setup & OpenGL Foundation
 * Team: Krithika & Akshatha
 * Developer: Krithika
 *
 * File: src/main.c
 * Description: Initial OpenGL and FreeGLUT window creation and basic rendering.
 */

#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include "camera.h"

/* Window dimensions */
static int window_width = 800;
static int window_height = 600;

/*
 * Initialize OpenGL state for 3D rendering.
 */
static void init_opengl(void) {
    /* Set background clear color (deep space darkness) */
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);

    /* Enable depth testing for 3D rendering */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    /* Configure initial projection matrix */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)window_width / (double)window_height, 0.1, 100.0);

    /* Return to modelview matrix */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*
 * Display callback: clears buffers, sets camera, and renders a 3D test primitive.
 */
static void display_callback(void) {
    /* Clear color and depth buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Set up Modelview matrix */
   camera_apply();

    /* Apply a slight tilt to demonstrate 3D depth and perspective */
    glPushMatrix();
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(30.0f, 0.0f, 1.0f, 0.0f);

    /* Render a simple 3D wireframe sphere to establish 3D geometry foundation */
    glColor3f(0.2f, 0.7f, 1.0f);
    glutWireSphere(1.2, 24, 16);

    glPopMatrix();

    /* Swap front and back buffers */
    glutSwapBuffers();
}

/*
 * Reshape callback: updates viewport and projection matrix when window is resized.
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
 * Keyboard callback: handles user key presses for clean exit.
 */
static void keyboard_callback(unsigned char key, int x, int y) {
    (void)x;
    (void)y;
    /* Exit cleanly on ESC (key code 27) or 'q' / 'Q' */
    if (key == 27 || key == 'q' || key == 'Q') {
        printf("[SolarSim] Clean exit requested by user.\n");
        glutLeaveMainLoop();
    }
    camera_keyboard(key);
glutPostRedisplay();
}

/*
 * Main entry point.
 */
int main(int argc, char** argv) {
    printf("====================================================\n");
    printf(" 3D Solar System & Space Exploration Simulator\n");
    printf(" Week 1: OpenGL & FreeGLUT Foundation\n");
    printf(" Team: Krithika & Akshatha\n");
    printf("====================================================\n");

    /* 1. Initialise FreeGLUT */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(100, 100);

    /* 2. Create window */
    glutCreateWindow("Solar System Simulator - Week 1 Foundation");

    /* 3. Initialize OpenGL 3D settings (depth test, projection, clear color) */
    init_opengl();
    camera_init();


    /* 4. Register callbacks */
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    glutKeyboardFunc(keyboard_callback);

    /* Allow clean return from main loop on window close */
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    printf("[SolarSim] Window created. Press ESC or 'q' to exit.\n");

    /* 5. Enter FreeGLUT event loop */
    glutMainLoop();

    printf("[SolarSim] Application terminated cleanly.\n");
    return EXIT_SUCCESS;
}
