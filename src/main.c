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

/* Window dimensions */
static int window_width = 800;
static int window_height = 600;

/*
 * Display callback: clears the buffers and renders an initial test object.
 */
static void display_callback(void) {
    /* Clear color and depth buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Reset modelview matrix */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* Render a simple colored 2D/3D test primitive (initial rendering test) */
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.3f, 0.2f); /* Red/Orange vertex */
        glVertex3f(-0.5f, -0.5f, 0.0f);
        glColor3f(0.2f, 0.8f, 0.3f); /* Green vertex */
        glVertex3f(0.5f, -0.5f, 0.0f);
        glColor3f(0.2f, 0.4f, 1.0f); /* Blue vertex */
        glVertex3f(0.0f, 0.5f, 0.0f);
    glEnd();

    /* Swap front and back buffers */
    glutSwapBuffers();
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

    /* 3. Set background clear color (deep space darkness) */
    glClearColor(0.05f, 0.05f, 0.1f, 1.0f);

    /* 4. Register callbacks */
    glutDisplayFunc(display_callback);
    glutKeyboardFunc(keyboard_callback);

    /* Allow clean return from main loop on window close */
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    printf("[SolarSim] Window created. Press ESC or 'q' to exit.\n");

    /* 5. Enter FreeGLUT event loop */
    glutMainLoop();

    printf("[SolarSim] Application terminated cleanly.\n");
    return EXIT_SUCCESS;
}
