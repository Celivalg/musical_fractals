#include "context.h"
#include "gui.h"

int main(int argc, char **argv) {

    GtkApplication *app;
    int stat = 0;

    struct context *context = init_context();

    app = gtk_application_new(NULL, G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(create_window), context);

    stat = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    free_context(context);
    return stat;
}

