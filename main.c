#include "controls.h"
#include "pdf_view.h"
#include "pdf_events.h"

int main(int argc, char *argv[]) {
    // Initialize the GTK application
    gtk_init(&argc, &argv);

    // Create a new top-level window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "pdfc");
    gtk_window_set_default_size(GTK_WINDOW(window), 420, 595);
    gtk_window_set_resizable(GTK_WINDOW(window), TRUE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL); // Connect the destroy signal to quit the application

    GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    // Create a header bar for the window
    GtkWidget *header_bar = gtk_header_bar_new();
    gtk_header_bar_set_title(GTK_HEADER_BAR(header_bar), "PDFC");
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header_bar), TRUE);
    gtk_box_pack_start(GTK_BOX(main_box), header_bar, FALSE, FALSE, 0);

    // Set up controls in the header bar (e.g., buttons, menus)
    setup_controls(header_bar);
    setup_menu(header_bar);

    // Initially disable controls until a PDF is loaded
    update_control_sensitivity(FALSE);
                                   
    // Create a scrolled window to allow scrolling of the PDF viewer
    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(main_box), scrolled_window, TRUE, TRUE, 0);

    // Create a drawing area for rendering the PDF
    pdf_viewer_data.drawingArea = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(scrolled_window), pdf_viewer_data.drawingArea);
    // Set the size request for the drawing area based on the PDF dimensions and zoom level
    gtk_widget_set_size_request(pdf_viewer_data.drawingArea,
            pdf_viewer_data.width * pdf_viewer_data.zoom_level,
            pdf_viewer_data.height * pdf_viewer_data.zoom_level);

    // Allow the drawing area to expand and center it in the scrolled window
    gtk_widget_set_vexpand(pdf_viewer_data.drawingArea, TRUE);
    gtk_widget_set_hexpand(pdf_viewer_data.drawingArea, TRUE);
    gtk_widget_set_halign(pdf_viewer_data.drawingArea, GTK_ALIGN_CENTER);

    gtk_widget_add_events(pdf_viewer_data.drawingArea, 
            GDK_SCROLL_MASK | GDK_BUTTON_PRESS_MASK | 
            GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK);

    // Connect event signals
    g_signal_connect(pdf_viewer_data.drawingArea, "scroll-event", G_CALLBACK(on_scroll), NULL);
    g_signal_connect(pdf_viewer_data.drawingArea, "button-press-event", G_CALLBACK(on_button_press), NULL);
    g_signal_connect(pdf_viewer_data.drawingArea, "button-release-event", G_CALLBACK(on_button_release), NULL);
    g_signal_connect(pdf_viewer_data.drawingArea, "motion-notify-event", G_CALLBACK(on_motion_notify), NULL);
    pdf_viewer_data.is_dragging = FALSE;
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);

    // If a PDF file path is provided as a command-line argument, initialize the application data
    if(argc > 1){
        update_control_sensitivity(TRUE);

        // Initialize application data with the provided PDF file path
        initialize_app_data(argv[1]);
    }

    // Connect the draw signal to the drawing function for rendering the PDF
    g_signal_connect(pdf_viewer_data.drawingArea, "draw", G_CALLBACK(on_draw), NULL);
    
    gtk_widget_show_all(window);
    gtk_main();

    // Clean up application data before exiting
    cleanup_app_data();
    return 0;
}

