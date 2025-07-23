#ifndef CONTROLS_H
#define CONTROLS_H

#include <gtk/gtk.h>

typedef struct {
    GtkWidget *previous_button;
    GtkWidget *next_button;
    GtkWidget *entry;
    GtkWidget *zoom_in_button;
    GtkWidget *zoom_out_button;
} Controls;

extern Controls controls;

void on_zoom_in_button_clicked(GtkWidget *widget, gpointer user_data);
void on_zoom_out_button_clicked(GtkWidget *widget, gpointer user_data);
void entry_activated(GtkWidget *entry, gpointer data);
void open_file_chooser(GtkWidget *widget, gpointer user_data);
void set_theme(GtkWidget *widget, gpointer user_data);
void setup_controls(GtkWidget *header_bar);
void setup_menu(GtkWidget *header_bar);
void update_control_sensitivity(gboolean condition);


#endif // CONTROLS_H

