#include "mouse_input.h"
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

    XGrabPointer(X_display, X_window, false, PointerMotionMask, GrabModeAsync,
                 GrabModeAsync, X_window, None, CurrentTime);

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
