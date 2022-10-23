#ifndef SCALE_MENU_H
#define SCALE_MENU_H

#include "context.h"
#include <gtk/gtk.h>

struct scale_builder {
    const char *menu_id;
    GCallback callback;
    double min;
    double max;
    double step;
    double initial;
    const char *label;
};

GtkRange *create_new_scale(GtkBuilder *builder, struct scale_builder scale,
                           struct context *context);

#endif
