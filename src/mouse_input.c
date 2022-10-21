#include "mouse_input.h"
#include "camera.h"
#include <X11/Xlib.h>
#include <gdk/x11/gdkx.h>
#include <gtk/gtk.h>

// Because gtk4 decided that no, you shouldn't touch the cursor,
// we have to do this by hand... because of course you never have to handle axis
// input in graphical applications, nooo of course not... thanks a lot gtk,
// really helpful
//
// unless there is a far better way of doing that, but I am personally not aware
// of one, and google wasn't either as far as I could tell...

bool grab_mouse(struct context *context) {

    GdkDisplay *gdk_display = gtk_widget_get_display(context->gtk_context->win);

    Display *X_display = gdk_x11_display_get_xdisplay(gdk_display);
    Window X_window = gdk_x11_display_get_xrootwindow(gdk_display);

    GdkCursor *cursor = gdk_cursor_new_from_name("none", NULL);
    gtk_widget_set_cursor(context->gtk_context->win, cursor);

    XGrabPointer(X_display, X_window, true, PointerMotionMask, GrabModeAsync,
                 GrabModeAsync, X_window, None, CurrentTime);
    warp_mouse(context);
    context->gtk_context->pointer_grabbed = true;
    return true;
}

void ungrab_mouse(struct context *context) {

    GdkDisplay *gdk_display = gtk_widget_get_display(context->gtk_context->win);
    Display *X_display = gdk_x11_display_get_xdisplay(gdk_display);

    gtk_widget_set_cursor(context->gtk_context->win, NULL);

    XUngrabPointer(X_display, CurrentTime);

    context->gtk_context->pointer_grabbed = false;
}

// keeps pointer in the middle
void warp_mouse(struct context *context) {

    GdkDisplay *gdk_display = gtk_widget_get_display(context->gtk_context->win);
    Display *X_display = gdk_x11_display_get_xdisplay(gdk_display);
    // Window X_window = gdk_x11_display_get_xrootwindow(gdk_display);

    GdkSurface *gdk_surf = gdk_gl_context_get_surface(
        gtk_gl_area_get_context(context->gtk_context->area));
    Window X_window = GDK_SURFACE_XID(gdk_surf);

    XWindowAttributes window_data;
    XGetWindowAttributes(X_display, X_window, &window_data);

    XWarpPointer(X_display, None, X_window, 0, 0, 0, 0, window_data.width / 2,
                 window_data.height / 2);
}

static void motion_mouse(__attribute__((unused)) GtkEventControllerMotion *self,
                         gdouble x, gdouble y, struct context *context) {
    if (!context->gtk_context->pointer_grabbed)
        return;
    GdkSurface *gdk_surf = gdk_gl_context_get_surface(
        gtk_gl_area_get_context(context->gtk_context->area));
    int middle_x = gdk_surface_get_width(gdk_surf) / 2;
    int middle_y = gdk_surface_get_height(gdk_surf) / 2;
    if (middle_x * 1000 == (int)(x * 1000) &&
        middle_y * 1000 == (int)(y * 1000))
        return;
    float new_x = (float)(middle_x - x);
    float new_y = (float)(middle_y - y);
    // printf("movement: x:%f y:%f\n", new_x, new_y);
    warp_mouse(context);
    mouse_to_angle(context, new_x, new_y);
}

GtkEventController *mouse_event_controller(struct context *context) {
    GtkEventController *controller = gtk_event_controller_motion_new();
    g_signal_connect(controller, "motion", G_CALLBACK(motion_mouse), context);
    return controller;
}
