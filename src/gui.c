#include "gui.h"
#include "context.h"
#include "key_events.h"
#include "mouse_input.h"
#include "render.h"
#include <math.h>
#include <stdbool.h>

void create_window(GApplication *app, struct context *context) {

    GtkBuilder *builder = gtk_builder_new_from_file("./ui/musical_fractals.ui");
    GtkWidget *win = GTK_WIDGET(gtk_builder_get_object(builder, "top_window"));
    if (!win) {
        printf("Failed to get window\n");
        exit(1);
    }
    gtk_window_set_application(GTK_WINDOW(win), GTK_APPLICATION(app));

    context->gtk_context->win = GTK_WIDGET(win);

    GtkEventController *k_controller = new_event_controller(context);
    GtkEventController *m_controller = mouse_event_controller(context);
    gtk_widget_add_controller(GTK_WIDGET(win), k_controller);
    gtk_widget_add_controller(GTK_WIDGET(win), m_controller);

    GtkWidget *gl_area = GTK_WIDGET(gtk_builder_get_object(builder, "gl_area"));
    context->gtk_context->area = GTK_GL_AREA(gl_area);
    setup_glarea(context);

    GtkWidget *paned =
        GTK_WIDGET(gtk_builder_get_object(builder, "paned_menu"));
    context->gtk_context->anim_data.paned = paned;

    gtk_widget_show(GTK_WIDGET(win));
}

