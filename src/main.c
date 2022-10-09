#include "gui.h"

static void app_activate(__attribute__((unused)) GApplication *app,
                         __attribute__((unused)) gpointer user_data) {

    create_window(app);
}

int main(int argc, char **argv) {

    GtkApplication *app;
    int stat = 0;

    app = gtk_application_new(NULL, G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);

    stat = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return stat;
}

