#ifndef CAMERA_H
#define CAMERA_H

#include "context.h"
#include <GL/glew.h>

void mouse_to_angle(struct context *context, float x, float y);

void key_forward(struct context *context, enum direction dir, bool pressed);

void calc_camera(struct context *context);

#endif
