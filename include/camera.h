#ifndef CAMERA_H
#define CAMERA_H

/* Initialize the camera */
void camera_init(void);

/* Handle keyboard movement */
void camera_keyboard(unsigned char key);

/* Apply the camera view */
void camera_apply(void);

/* Reset camera to its starting position */
void camera_reset(void);

/* Focus camera on a selected planet */
void camera_focus_planet(int index);

/* Return camera to the full Solar System view */
void camera_clear_focus(void);
#endif