#include "gui.h"
#include "context.h"
#include "gl_load_shaders.h"
#include "key_events.h"
#include "mouse_input.h"
#include "render.h"
#include "scale_menu.h"
#include <math.h>
#include <stdbool.h>

void add_scale_max_step(GtkBuilder *builder, struct context *context);
void add_scale_max_dist(GtkBuilder *builder, struct context *context);
void add_scale_surface_tresh(GtkBuilder *builder, struct context *context);

void create_window(GApplication *app, struct context *context) {
    // honestly, I should check for if we managed to get the other widgets, but
    // it's probably gonna crash anyway so
    GtkBuilder *builder =
        gtk_builder_new_from_file("./ui/musical_fractals.xml");
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

    GtkWidget *reload_shaders_b =
        GTK_WIDGET(gtk_builder_get_object(builder, "reload_shaders"));
    g_signal_connect(reload_shaders_b, "clicked",
                     G_CALLBACK(load_default_shaders), context);

    add_scale_max_step(builder, context);
    add_scale_max_dist(builder, context);
    add_scale_surface_tresh(builder, context);

    gtk_widget_show(GTK_WIDGET(win));
}

// -- max_step scale -- //
void update_max_step(GtkRange *self, struct context *context) {
    double v = gtk_range_get_value(self);
    context->drawing.max_step = (unsigned int)pow(10.0, v);
}

void add_scale_max_step(GtkBuilder *builder, struct context *context) {
    struct scale_builder scale = {.menu_id = "ray_marching",
                                  .callback = G_CALLBACK(update_max_step),
                                  .min = 0.0,
                                  .max = 10.0,
                                  .step = 0.01,
                                  .initial = 2.0,
                                  .label = "Maximum Steps"};
    create_new_scale(builder, scale, context);
}

// -- max_dist scale -- //
void update_max_dist(GtkRange *self, struct context *context) {
    double v = gtk_range_get_value(self);
    context->drawing.max_dist = (float)pow(10.0, v);
}

void add_scale_max_dist(GtkBuilder *builder, struct context *context) {
    struct scale_builder scale = {.menu_id = "ray_marching",
                                  .callback = G_CALLBACK(update_max_dist),
                                  .min = 0.0,
                                  .max = 10.0,
                                  .step = 0.01,
                                  .initial = 2.0,
                                  .label = "Maximum Distance"};
    create_new_scale(builder, scale, context);
}

// -- surface_tresh scale -- //
void update_surface_tresh(GtkRange *self, struct context *context) {
    double v = gtk_range_get_value(self);
    context->drawing.surface_tresh = (float)(1.0 / pow(10.0, v));
}

void add_scale_surface_tresh(GtkBuilder *builder, struct context *context) {
    struct scale_builder scale = {.menu_id = "ray_marching",
                                  .callback = G_CALLBACK(update_surface_tresh),
                                  .min = 0.0,
                                  .max = 10.0,
                                  .step = 0.01,
                                  .initial = 2.0,
                                  .label = "Surface Treshold"};
    create_new_scale(builder, scale, context);
}
