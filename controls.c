#include <gtk/gtk.h>
#include "controls.h"
#include "pdf_view.h"

Controls controls = {NULL, NULL, NULL, NULL, NULL};

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

// Function to open a file chooser dialog for selecting a PDF file
void open_file_chooser(GtkWidget *widget, gpointer user_data) {
    GtkWidget *dialog;
    dialog = gtk_file_chooser_dialog_new("Open File",
                                         GTK_WINDOW(user_data),
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Open", GTK_RESPONSE_ACCEPT,
                                         NULL);

    if (dialog == NULL) {
        g_printerr("Failed to create file chooser dialog.\n");
        return;
    }

    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "PDF Files");
    gtk_file_filter_add_mime_type(filter, "application/pdf");
    gtk_file_filter_add_pattern(filter, "*.pdf");

    // Add the filter to the dialog
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    // Set the default filter to the PDF filter
    gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), filter);

    // Run the dialog and check if the user accepted the file selection
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        if (filename) {
            //g_print("Selected file: %s\n", filename);
            initialize_app_data(filename); // Initialize application data with the selected file
            g_free(filename);
        } else {
            g_printerr("No file was selected.\n");
        }
    }

    gtk_widget_destroy(dialog);
}

// Function to set the application theme (light or dark mode)
void set_theme(GtkWidget *widget, gpointer user_data) {
    GtkSettings *settings = gtk_settings_get_default();
    char *data = (char *)user_data;
    if(strcmp(data, "dark") == 0){
        g_object_set(settings, "gtk-theme-name", "Adwaita-dark", NULL);
    } else {
        g_object_set(settings, "gtk-theme-name", "Adwaita", NULL);
    }
}


// Function to set up controls in the header bar
void setup_controls(GtkWidget *header_bar) {
    // Create a button for going to the previous page
    controls.previous_button = gtk_button_new();
    GtkWidget *previous_image = gtk_image_new_from_icon_name("go-previous", GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(controls.previous_button), previous_image);
    g_signal_connect(controls.previous_button, "clicked", G_CALLBACK(previous_page), NULL);
    gtk_widget_set_size_request(controls.previous_button, 25, 25);
    
    // Create a button for going to the next page
    controls.next_button = gtk_button_new();
    GtkWidget *next_image = gtk_image_new_from_icon_name("go-next", GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(controls.next_button), next_image);
    g_signal_connect(controls.next_button, "clicked", G_CALLBACK(next_page), NULL);
    gtk_widget_set_size_request(controls.next_button, 25, 25);

    // Create an entry field for page number input
    controls.entry = gtk_entry_new();
    g_signal_connect(controls.entry, "activate", G_CALLBACK(entry_activated), NULL);
    gtk_widget_set_size_request(controls.entry, 1, 25);

    // Create a button for zooming in
    controls.zoom_in_button = gtk_button_new();
    GtkWidget *zoom_in_image = gtk_image_new_from_icon_name("list-add", GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(controls.zoom_in_button), zoom_in_image);
    g_signal_connect(controls.zoom_in_button, "clicked", G_CALLBACK(on_zoom_in_button_clicked), NULL);
    gtk_widget_set_size_request(controls.zoom_in_button, 25, 25);

    // Create a button for zooming out
    controls.zoom_out_button = gtk_button_new();
    GtkWidget *zoom_out_image = gtk_image_new_from_icon_name("list-remove", GTK_ICON_SIZE_BUTTON);
    gtk_button_set_image(GTK_BUTTON(controls.zoom_out_button), zoom_out_image);
    g_signal_connect(controls.zoom_out_button, "clicked", G_CALLBACK(on_zoom_out_button_clicked), NULL);
    gtk_widget_set_size_request(controls.zoom_out_button, 25, 25);

    // Pack all the created widgets into the header bar
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), controls.previous_button);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), controls.next_button);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), controls.entry);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), controls.zoom_in_button);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), controls.zoom_out_button);
}

// Function to set up the menu in the header bar
void setup_menu(GtkWidget *header_bar) {
    // Create a menu
    GtkWidget *menu = gtk_menu_new();

    // Create menu item for file chooser
    GtkWidget *file_choose_button = gtk_menu_item_new_with_label("Choose File");
    g_signal_connect(file_choose_button, "activate", G_CALLBACK(open_file_chooser), NULL);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), file_choose_button);

    // Create a submenu for themes
    GtkWidget *themes_menu = gtk_menu_new();
    GtkWidget *themes_item = gtk_menu_item_new_with_label("Themes");
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), themes_item);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(themes_item), themes_menu);

    // Create light mode and dark mode menu items
    GtkWidget *light_mode_item = gtk_menu_item_new_with_label("Light Mode");
    g_signal_connect(light_mode_item, "activate", G_CALLBACK(set_theme), "light");
    gtk_menu_shell_append(GTK_MENU_SHELL(themes_menu), light_mode_item);

    GtkWidget *dark_mode_item = gtk_menu_item_new_with_label("Dark Mode");
    g_signal_connect(dark_mode_item, "activate", G_CALLBACK(set_theme), "dark");
    gtk_menu_shell_append(GTK_MENU_SHELL(themes_menu), dark_mode_item);

    // Show the menu items
    gtk_widget_show_all(menu);

    // Create a button for the menu
    GtkWidget *menu_button = gtk_menu_button_new();
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), menu_button);

    // Associate the menu with the menu button
    gtk_menu_button_set_popup(GTK_MENU_BUTTON(menu_button), GTK_WIDGET(menu));

}

// Function to enable or disable the control buttons and entry field
void update_control_sensitivity(gboolean condition) {
    gtk_widget_set_sensitive(controls.previous_button, condition);
    gtk_widget_set_sensitive(controls.next_button, condition);
    gtk_widget_set_sensitive(controls.entry, condition);
    gtk_widget_set_sensitive(controls.zoom_in_button, condition);
    gtk_widget_set_sensitive(controls.zoom_out_button, condition);
}
