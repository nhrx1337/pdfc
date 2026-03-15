#include "pdf_events.h"

// Handle mouse scroll events (Ctrl + Scroll for zooming)
gboolean on_scroll(GtkWidget *widget, GdkEventScroll *event, gpointer data) {
    if (event->state & GDK_CONTROL_MASK) {
        if (event->direction == GDK_SCROLL_UP) {
            on_zoom_in_button_clicked(NULL, NULL);
        } else if (event->direction == GDK_SCROLL_DOWN) {
            on_zoom_out_button_clicked(NULL, NULL);
        }
        return TRUE; // Event handled
    }
    return FALSE; // Allow normal scrolling if Ctrl is not pressed
}

// Handle mouse button press (Initiate dragging)
gboolean on_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    if (event->button == 1) { // Left click
        pdf_viewer_data.is_dragging = TRUE;
        pdf_viewer_data.last_x = event->x;
        pdf_viewer_data.last_y = event->y;

        // Change cursor to indicate dragging
        GdkWindow *window = gtk_widget_get_window(widget);
        GdkCursor *cursor = gdk_cursor_new_for_display(gdk_display_get_default(), GDK_FLEUR);
        gdk_window_set_cursor(window, cursor);
        g_object_unref(cursor);
    }
    return TRUE;
}

// Handle mouse button release (End dragging)
gboolean on_button_release(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    if (event->button == 1) {
        pdf_viewer_data.is_dragging = FALSE;
        // Reset cursor to default
        gdk_window_set_cursor(gtk_widget_get_window(widget), NULL); 
    }
    return TRUE;
}

// Handle mouse motion (Perform view panning)
gboolean on_motion_notify(GtkWidget *widget, GdkEventMotion *event, gpointer data) {
    if (pdf_viewer_data.is_dragging) {
        // Find the ScrolledWindow ancestor to update scroll adjustments
        GtkWidget *scrolled_window = gtk_widget_get_ancestor(widget, GTK_TYPE_SCROLLED_WINDOW);
        if (scrolled_window) {
            GtkAdjustment *hadj = gtk_scrolled_window_get_hadjustment(GTK_SCROLLED_WINDOW(scrolled_window));
            GtkAdjustment *vadj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolled_window));

            // Calculate movement delta
            double dx = event->x - pdf_viewer_data.last_x;
            double dy = event->y - pdf_viewer_data.last_y;

            // Apply movement to scrollbars
            gtk_adjustment_set_value(hadj, gtk_adjustment_get_value(hadj) - dx);
            gtk_adjustment_set_value(vadj, gtk_adjustment_get_value(vadj) - dy);

            return TRUE;
        }
    }
    return FALSE;
}

// Handle keyboard shortcuts (Left/Right arrows for navigation)
gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    if (event->keyval == GDK_KEY_Left) {
        previous_page(NULL, NULL);
        return TRUE; // Stop event propagation
    } else if (event->keyval == GDK_KEY_Right) {
        next_page(NULL, NULL);
        return TRUE; // Stop event propagation
    } else if (event->keyval == GDK_KEY_F11) {
        static gboolean is_fullscreen = FALSE;
        GtkWindow *win = GTK_WINDOW(gtk_widget_get_toplevel(widget));
        if (!is_fullscreen) {
            gtk_window_fullscreen(win);
        } else {
            gtk_window_unfullscreen(win);
        }
        is_fullscreen = !is_fullscreen;
        return TRUE;
    } else if (event->keyval == GDK_KEY_Up || event->keyval == GDK_KEY_Down) {
        GtkWidget *scrolled_window = gtk_widget_get_ancestor(pdf_viewer_data.drawingArea, GTK_TYPE_SCROLLED_WINDOW);
        
        if (scrolled_window) {
            GtkAdjustment *vadj = gtk_scrolled_window_get_vadjustment(GTK_SCROLLED_WINDOW(scrolled_window));
            double current_pos = gtk_adjustment_get_value(vadj);
            double step = gtk_adjustment_get_step_increment(vadj) * 2; // Scroll speed

            if (event->keyval == GDK_KEY_Up) {
                gtk_adjustment_set_value(vadj, current_pos - step);
            } else {
                gtk_adjustment_set_value(vadj, current_pos + step);
            }
            return TRUE;
        }
    } else if (event->keyval == GDK_KEY_f || event->keyval == GDK_KEY_F) {
        fit_to_height();
        return TRUE;
    }
    return FALSE; // Propagate other keys
}
