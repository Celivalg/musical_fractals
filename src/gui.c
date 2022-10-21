#include "gui.h"
#include "context.h"
#include "key_events.h"
#include "mouse_input.h"
#include "render.h"

void create_window(GApplication *app) {

    /*GtkWidget *win;
    win = gtk_application_window_new(GTK_APPLICATION(app));

    gtk_window_set_title(GTK_WINDOW(win), "Fractal Viewer");
    gtk_window_set_default_size(GTK_WINDOW(win), 400, 300);

    // Allocating memory for data on position of GlContext data
    struct context *context = init_context();
    context->gtk_context->win = win;
    GtkEventController *k_controller = new_event_controller(context);
    GtkEventController *m_controller = mouse_event_controller(context);
    gtk_widget_add_controller(win, k_controller);
    gtk_widget_add_controller(win, m_controller);

    GtkWidget *hpaned = gtk_paned_new(GTK_ORIENTATION_HORIZONTAL);
    GtkWidget *glarea = setup_glarea(context);
    GtkWidget *frame2 = gtk_frame_new(NULL);

    gtk_widget_set_size_request(hpaned, 200, -1);

    gtk_paned_set_start_child(GTK_PANED(hpaned), glarea);
    gtk_paned_set_resize_start_child(GTK_PANED(hpaned), TRUE);
    gtk_paned_set_shrink_start_child(GTK_PANED(hpaned), FALSE);
    gtk_widget_set_size_request(glarea, 50, -1);

    gtk_paned_set_end_child(GTK_PANED(hpaned), frame2);
    gtk_paned_set_resize_end_child(GTK_PANED(hpaned), FALSE);
    gtk_paned_set_shrink_end_child(GTK_PANED(hpaned), FALSE);
    gtk_widget_set_size_request(frame2, 50, -1);

    gtk_window_set_child(GTK_WINDOW(win), hpaned);*/

    GtkBuilder *builder = gtk_builder_new_from_file("./ui/musical_fractals.ui");
    GtkWidget *win = GTK_WIDGET(gtk_builder_get_object(builder, "top_window"));
    if (!win) {
        printf("Failed to get window\n");
        exit(1);
    }
    gtk_window_set_application(GTK_WINDOW(win), GTK_APPLICATION(app));

    // Allocating memory for data on position of GlContext data
    struct context *context = init_context();
    context->gtk_context->win = GTK_WIDGET(win);
    GtkEventController *k_controller = new_event_controller(context);
    GtkEventController *m_controller = mouse_event_controller(context);
    gtk_widget_add_controller(GTK_WIDGET(win), k_controller);
    gtk_widget_add_controller(GTK_WIDGET(win), m_controller);

    GtkWidget *gl_area = GTK_WIDGET(gtk_builder_get_object(builder, "gl_area"));
    context->gtk_context->area = GTK_GL_AREA(gl_area);
    setup_glarea(context);

    gtk_widget_show(GTK_WIDGET(win));
}
