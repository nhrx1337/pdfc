#include "pdf_view.h"

PdfViewerData pdf_viewer_data = {NULL, 0, NULL, NULL, NULL, 420, 595, 1.0};

// Callback function to handle drawing the PDF page
gboolean on_draw(GtkWidget *widget, cairo_t *cr) {
    if (pdf_viewer_data.page) {
        GtkAllocation allocation;
        gtk_widget_get_allocation(widget, &allocation); // Get the allocation of the widget (size)

        double page_width, page_height;
        poppler_page_get_size(pdf_viewer_data.page, &page_width, &page_height);

        // Calculate scale factor for width and height, Use the smaller scale to maintain aspect ratio
        double scale = fmin(allocation.width / page_width, allocation.height / page_height);

        cairo_save(cr); // Save the current state of the Cairo context
        cairo_scale(cr, scale, scale);
        poppler_page_render(pdf_viewer_data.page, cr); // Render the PDF page onto the Cairo context
        cairo_restore(cr); // Restore the previous state of the Cairo context
    }
    return FALSE; // Return FALSE to indicate that the default handler should be called
}

// Function to load a specific page of the PDF document
void load_page(int page_number) {
    if (!pdf_viewer_data.document) {
        g_printerr("Error: No document loaded.\n");
        return;
    }

    if (pdf_viewer_data.page) {
        g_object_unref(pdf_viewer_data.page);
    }

    pdf_viewer_data.page = poppler_document_get_page(pdf_viewer_data.document, page_number);
    if (!pdf_viewer_data.page) {
        g_printerr("Error: Could not load page %d.\n", page_number);
        return;
    }

    if (pdf_viewer_data.drawingArea) {
        gtk_widget_queue_draw(pdf_viewer_data.drawingArea);
    }

    if (pdf_viewer_data.entry) {
        gtk_entry_set_text(GTK_ENTRY(pdf_viewer_data.entry), g_strdup_printf("%d", pdf_viewer_data.curr_page));
    }
}

// Function to navigate to the next page
void next_page(GtkWidget *widget, gpointer user_data) {
    if (pdf_viewer_data.document && pdf_viewer_data.curr_page < poppler_document_get_n_pages(pdf_viewer_data.document) - 1) {
        pdf_viewer_data.curr_page++;
        load_page(pdf_viewer_data.curr_page);
    }
}

// Function to navigate to the previous page
void previous_page(GtkWidget *widget, gpointer user_data) {
    if (pdf_viewer_data.document && pdf_viewer_data.curr_page > 0) {
        pdf_viewer_data.curr_page--;
        load_page(pdf_viewer_data.curr_page);
    }
}

// Function to initialize application data with the PDF file
void initialize_app_data(const char *file_path) {
    GError *error = NULL;
    pdf_viewer_data.document = poppler_document_new_from_file(file_path, NULL, &error);
    if (error) {
        g_printerr("Error opening PDF file: %s\n", error->message);
        g_error_free(error);
        return;
    }
    load_page(pdf_viewer_data.curr_page);
}

// Function to clean up application data
void cleanup_app_data() {
    if (pdf_viewer_data.page) {
        g_object_unref(pdf_viewer_data.page);
    }
    if (pdf_viewer_data.document) {
        g_object_unref(pdf_viewer_data.document);
    }
}

// Function to update the size of the drawing area based on zoom level
void update_drawing_area_size() {
    // Set the size request for the drawing area based on the current zoom level
    gtk_widget_set_size_request(pdf_viewer_data.drawingArea, pdf_viewer_data.width * pdf_viewer_data.zoom_level, pdf_viewer_data.height * pdf_viewer_data.zoom_level);
    gtk_widget_queue_draw(pdf_viewer_data.drawingArea);
}

