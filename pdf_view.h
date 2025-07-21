#ifndef PDF_VIEW_H
#define PDF_VIEW_H

#include <gtk/gtk.h>
#include <poppler/glib/poppler.h>
#include <math.h>

typedef struct {
    PopplerDocument *document;
    int curr_page;
    PopplerPage *page;
    GtkWidget *drawingArea;
    GtkWidget *entry;
    int width;  
    int height;
    double zoom_level; 
} PdfViewerData; 

extern PdfViewerData pdf_viewer_data;

gboolean on_draw(GtkWidget *widget, cairo_t *cr);
void load_page(int page_number);
void next_page(GtkWidget *widget, gpointer user_data);
void previous_page(GtkWidget *widget, gpointer user_data);
void initialize_app_data(const char *file_path);
void cleanup_app_data();
void update_drawing_area_size();

#endif // PDF_VIEW_H

