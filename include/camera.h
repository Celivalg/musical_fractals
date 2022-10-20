#ifndef CAMERA_H
#define CAMERA_H

#include "context.h"
#include <GL/glew.h>

void key_forward(struct context *context, enum direction dir, bool pressed);

void calc_camera(struct context *context);

#endif
