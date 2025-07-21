#include <gtk/gtk.h>
#include "controls.h"
#include "pdf_view.h"

void on_zoom_in_button_clicked(GtkWidget *widget, gpointer user_data) {
    pdf_viewer_data.zoom_level *= 1.2; // Increase zoom level by 20%
    update_drawing_area_size(); // Update the drawing area size based on the new zoom level
}

void on_zoom_out_button_clicked(GtkWidget *widget, gpointer user_data) {
    pdf_viewer_data.zoom_level /= 1.2; // Decrease zoom level by 20%
    update_drawing_area_size(); 
}

// Callback function for when the entry field is activated (e.g., when the user presses Enter)
void entry_activated(GtkWidget *entry, gpointer data) {
    const gchar *text = gtk_entry_get_text(GTK_ENTRY(entry)); // Get the text from the entry field

    char *endptr;
    long value = strtol(text, &endptr, 10); // Convert the text to a long integer
    int last_page = poppler_document_get_n_pages(pdf_viewer_data.document) - 1; // Get the last page index

    // Check if the input is a valid number and within the range
    if (*endptr == '\0' && endptr != text) {
        if (value >= 0 && value <= last_page) {
            // Valid page number, load the specified page
            load_page(pdf_viewer_data.curr_page = value);
        } else {
            // Out of range, reset the entry to the current page number
            gtk_entry_set_text(GTK_ENTRY(entry), g_strdup_printf("%d", pdf_viewer_data.curr_page));
        }
    } else {
        // Invalid input, reset the entry to the current page number
        gtk_entry_set_text(GTK_ENTRY(entry), g_strdup_printf("%d", pdf_viewer_data.curr_page));
    }
}

// Function to set up controls in the header bar
void setup_controls(GtkWidget *header_bar) {

    // Create a button for going to the previous page
    GtkWidget *previous_button = gtk_button_new();
    GtkWidget *previous_image = gtk_image_new_from_icon_name("go-previous", GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(previous_button), previous_image);
    g_signal_connect(previous_button, "clicked", G_CALLBACK(previous_page), NULL);
    gtk_widget_set_size_request(previous_button, 50, 25);
    
    // Create a button for going to the next page
    GtkWidget *next_button = gtk_button_new();
    GtkWidget *next_image = gtk_image_new_from_icon_name("go-next", GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(next_button), next_image);
    g_signal_connect(next_button, "clicked", G_CALLBACK(next_page), NULL);
    gtk_widget_set_size_request(next_button, 50, 25);

    // Create an entry field for page number input
    pdf_viewer_data.entry = gtk_entry_new();
    g_signal_connect(pdf_viewer_data.entry, "activate", G_CALLBACK(entry_activated), NULL);
    gtk_widget_set_size_request(pdf_viewer_data.entry, 50, 25);

    // Create a button for zooming in
    GtkWidget *zoom_in_button = gtk_button_new();
    GtkWidget *zoom_in_image = gtk_image_new_from_icon_name("list-add", GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(zoom_in_button), zoom_in_image);
    g_signal_connect(zoom_in_button, "clicked", G_CALLBACK(on_zoom_in_button_clicked), NULL);
    gtk_widget_set_size_request(zoom_in_button, 50, 25);

    // Create a button for zooming out
    GtkWidget *zoom_out_button = gtk_button_new();
    GtkWidget *zoom_out_image = gtk_image_new_from_icon_name("list-remove", GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(zoom_out_button), zoom_out_image);
    g_signal_connect(zoom_out_button, "clicked", G_CALLBACK(on_zoom_out_button_clicked), NULL);
    gtk_widget_set_size_request(zoom_out_button, 50, 25);

    // Pack all the created widgets into the header bar
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), previous_button);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), next_button);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), pdf_viewer_data.entry);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), zoom_in_button);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), zoom_out_button);
}

