#include <GL/freeglut.h>
#include <stdio.h>
#include "ui.h"
#include "camera.h"
#include "planet.h"

/*
 * Week 7: Planet Information UI
 * Developer: Akshatha
 */

 static const char *planet_descriptions[] =
{
    "Closest planet to the Sun.",
    "The hottest planet in the Solar System.",
    "Our home planet.",
    "A rocky planet known as the Red Planet.",
    "The largest planet in the Solar System.",
    "A gas giant famous for its rings.",
    "An ice giant with a blue-green appearance.",
    "The farthest planet from the Sun."
};

static void draw_text(float x, float y, const char *text)
{
    glRasterPos2f(x, y);

    while (*text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text);
        text++;
    }
}

void ui_init(void)
{
    /* UI initialization */
}

void ui_render_planet_info(void)
{
    int selected_planet = camera_get_focused_planet();

    /* No planet selected */
    if (selected_planet < 0)
    {
        return;
    }

    const Planet *planet = planets_get(selected_planet);

    if (planet == NULL)
    {
        return;
    }

    /*
     * Save current OpenGL state
     */
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);

    /*
     * Switch to 2D screen coordinates
     */
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluOrtho2D(0, 1000, 0, 700);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    /*
     * UI should appear on top of the 3D scene.
     */
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

    /*
     * Information panel
     */
    glColor3f(0.05f, 0.05f, 0.08f);

    glBegin(GL_QUADS);
        glVertex2f(25, 470);
        glVertex2f(350, 470);
        glVertex2f(350, 665);
        glVertex2f(25, 665);
    glEnd();

    /*
     * Planet name
     */
    glColor3f(1.0f, 1.0f, 1.0f);

    char text[100];

    sprintf(text, "Planet: %s", planet->name);
    draw_text(45, 625, text);

    sprintf(text, "Info: %s", planet_descriptions[selected_planet]);
    draw_text(45, 495, text);

    /*
     * Planet order
     */
    sprintf(text, "Order from Sun: %d", selected_planet + 1);
    draw_text(45, 590, text);

    /*
     * Visual size used by the simulator
     */
    sprintf(text, "Relative size: %.2f", planet->radius);
    draw_text(45, 555, text);

    /*
     * Orbital distance used by the simulator
     */
    sprintf(text, "Orbital distance: %.1f", planet->orbit_distance);
    draw_text(45, 525, text);

    /*
     * Restore OpenGL state
     */
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glPopAttrib();
}