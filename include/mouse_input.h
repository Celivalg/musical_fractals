#ifndef MOUSE_INPUT_H
#define MOUSE_INPUT_H

#include "context.h"

bool grab_mouse(struct context *context);

void ungrab_mouse(struct context *context);

void warp_mouse(struct context *context);

#endif
