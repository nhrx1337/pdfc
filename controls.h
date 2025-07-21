#ifndef BUTTONS_H
#define BUTTONS_H

#include <gtk/gtk.h>

void on_zoom_in_button_clicked(GtkWidget *widget, gpointer user_data);
void on_zoom_out_button_clicked(GtkWidget *widget, gpointer user_data);
void entry_activated(GtkWidget *entry, gpointer data);
void setup_controls(GtkWidget *header_bar);

#endif // BUTTONS_H

