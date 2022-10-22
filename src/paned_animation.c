#include "paned_animation.h"

void move_paned(struct context *context) {
    struct paned_animation_data *data = &(context->gtk_context->anim_data);
    GtkWidget *win = context->gtk_context->win;
    int size = gtk_paned_get_position(GTK_PANED(data->paned));
    int win_width = gtk_widget_get_width(win);

    const int total_size = 200;

    if (data->is_closing) {
        if (size < win_width) {
            if (size + 10 >= win_width) {
                gtk_paned_set_position(GTK_PANED(data->paned), win_width);
                data->is_closing = false;
                data->is_open = false;
                data->is_closed = true;
                return;
            } else {
                gtk_paned_set_position(GTK_PANED(data->paned), size + 10);
                return;
            }
        } else {
            data->is_closing = false;
            data->is_open = false;
            data->is_closed = true;
            return;
        }
    } else if (data->is_opening) {
        if (size > win_width - total_size) {
            if (size - 10 <= win_width - total_size) {
                gtk_paned_set_position(GTK_PANED(data->paned),
                                       win_width - total_size);
                data->is_opening = false;
                data->is_closed = false;
                data->is_open = true;
                return;

            } else {
                gtk_paned_set_position(GTK_PANED(data->paned), size - 10);
                return;
            }
        } else {
            data->is_opening = false;
            data->is_closed = false;
            data->is_open = true;
            return;
        }
    }

    if (data->is_open && size != win_width - total_size) {
        gtk_paned_set_position(GTK_PANED(data->paned), win_width - total_size);
    } else if (data->is_closed && size != win_width) {
        gtk_paned_set_position(GTK_PANED(data->paned), win_width);
    }
}

void close_paned(struct context *context) {
    struct paned_animation_data *data = &(context->gtk_context->anim_data);
    data->is_opening = false;
    data->is_closing = true;
    data->is_open = false;
    data->is_closed = false;
}

void open_paned(struct context *context) {
    struct paned_animation_data *data = &(context->gtk_context->anim_data);
    data->is_opening = true;
    data->is_closing = false;
    data->is_open = false;
    data->is_closed = false;
}
