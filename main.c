#include <gtk/gtk.h>
#include "controls.h"
#include "pdf_view.h"

int main(int argc, char *argv[]) {
    //g_setenv("GTK_THEME", "Adwaita:light", TRUE);

    if (argc < 2) {
        g_printerr("Usage: %s file:/path/your/pdf\n", argv[0]);
        return 1;
    }

    // Initialize the GTK application
    gtk_init(&argc, &argv);
    initialize_app_data(argv[1]); // Initialize application data with the provided PDF file path

    // Create a new top-level window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "pdfc");
    gtk_window_set_default_size(GTK_WINDOW(window), 420, 595);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL); // Connect the destroy signal to quit the application

    // Create a header bar for the window
    GtkWidget *header_bar = gtk_header_bar_new();
    gtk_header_bar_set_title(GTK_HEADER_BAR(header_bar), "PDFC");
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header_bar), TRUE);
    gtk_window_set_titlebar(GTK_WINDOW(window), header_bar); // Set the header bar as the title bar of the window
    
    // Set up controls in the header bar (e.g., buttons, menus)
    setup_controls(header_bar);

     // Create vertical and horizontal adjustments for scrolling
    GtkAdjustment *vadjustment = gtk_adjustment_new(0, 0, pdf_viewer_data.height, 1, 10, 0);
    GtkAdjustment *hadjustment = gtk_adjustment_new(0, 0, pdf_viewer_data.width, 1, 10, 0);

    // Create a scrolled window to allow scrolling of the PDF viewer
    GtkWidget *scrolled_window = gtk_scrolled_window_new(hadjustment, vadjustment);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(window), scrolled_window);

    // Create a drawing area for rendering the PDF
    pdf_viewer_data.drawingArea = gtk_drawing_area_new();
    g_signal_connect(pdf_viewer_data.drawingArea, "draw", G_CALLBACK(on_draw), NULL); // Connect the draw signal to the drawing function                              
    gtk_container_add(GTK_CONTAINER(scrolled_window), pdf_viewer_data.drawingArea);
    // Set the size request for the drawing area based on the PDF dimensions and zoom level
    gtk_widget_set_size_request(pdf_viewer_data.drawingArea,
            pdf_viewer_data.width*pdf_viewer_data.zoom_level,
            pdf_viewer_data.height*pdf_viewer_data.zoom_level);

    // Allow the drawing area to expand and center it in the scrolled window
    gtk_widget_set_vexpand(pdf_viewer_data.drawingArea, TRUE);
    gtk_widget_set_hexpand(pdf_viewer_data.drawingArea, TRUE);
    gtk_widget_set_halign(pdf_viewer_data.drawingArea, GTK_ALIGN_CENTER);

    // Connect the size-allocate signal to update the drawing area size when the window is resized
    g_signal_connect(pdf_viewer_data.drawingArea, "size-allocate", G_CALLBACK(update_drawing_area_size), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    // Clean up application data before exiting
    cleanup_app_data();
    return 0;
}

