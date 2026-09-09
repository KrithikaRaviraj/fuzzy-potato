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

#endif