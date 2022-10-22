#include "scale_menu.h"

GtkWidget *create_label_box(GtkWidget *widget, const char *label_str) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
    GtkWidget *label = gtk_label_new(label_str);
    gtk_box_append(GTK_BOX(box), label);
    gtk_box_append(GTK_BOX(box), widget);
    return box;
}

void create_new_scale(GtkBuilder *builder, struct scale_builder scale,
                      struct context *context) {
    GtkListBox *menu =
        GTK_LIST_BOX(gtk_builder_get_object(builder, scale.menu_id));
    GtkWidget *scale_w = gtk_scale_new_with_range(
        GTK_ORIENTATION_HORIZONTAL, scale.min, scale.max, scale.step);

    gtk_scale_set_draw_value(GTK_SCALE(scale_w), true);
    gtk_scale_set_value_pos(GTK_SCALE(scale_w), GTK_POS_LEFT);
    gtk_range_set_value(GTK_RANGE(scale_w), scale.initial);

    g_signal_connect(scale_w, "value-changed", scale.callback, context);

    GtkWidget *labeled = create_label_box(scale_w, scale.label);
    gtk_list_box_append(menu, labeled);
}
