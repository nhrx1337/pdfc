#ifndef PDF_EVENTS_H
#define PDF_EVENTS_H

#include <gtk/gtk.h>
#include "pdf_view.h"

// Mouse event handlers
gboolean on_scroll(GtkWidget *widget, GdkEventScroll *event, gpointer data);
gboolean on_button_press(GtkWidget *widget, GdkEventButton *event, gpointer data);
gboolean on_button_release(GtkWidget *widget, GdkEventButton *event, gpointer data);
gboolean on_motion_notify(GtkWidget *widget, GdkEventMotion *event, gpointer data);

// Keyboard event handlers
gboolean on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer data);

#endif // PDF_EVENTS_H
