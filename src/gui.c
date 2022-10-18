#include "gui.h"
#include "context.h"
#include "render.h"

void create_window(GApplication *app) {

    GtkWidget *win;
    win = gtk_application_window_new(GTK_APPLICATION(app));

    gtk_window_set_title(GTK_WINDOW(win), "Fractal Viewer");
    gtk_window_set_default_size(GTK_WINDOW(win), 400, 300);

    // Allocating memory for data on position of GlContext data
    struct context *context = init_context();

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

    gtk_window_set_child(GTK_WINDOW(win), hpaned);

    gtk_widget_show(win);
}
