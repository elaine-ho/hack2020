#include <gtk/gtk.h>
#include <cairo.h>


int x;
int max_x;
int y;

gboolean on_draw(GtkWidget* widget, GdkEventExpose* event, gpointer data)
{
    GdkWindow* window = gtk_widget_get_window(widget);
    cairo_region_t* cairoRegion = cairo_region_create();
    GdkDrawingContext* drawingContext;
    drawingContext = gdk_window_begin_draw_frame(window, cairoRegion);

    cairo_t* cr = gdk_drawing_context_get_cairo_context(drawingContext);
    cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 0.0);

    int              w, h;
    cairo_surface_t* image;

    image = cairo_image_surface_create_from_png("Images/8.png");
    w = cairo_image_surface_get_width(image);
    h = cairo_image_surface_get_height(image);

    cairo_scale(cr, 2, 2);

    int y_pos = y / 2 - h;

    cairo_set_source_surface(cr, image, x, y_pos);
    cairo_paint(cr);
    cairo_surface_destroy(image);

    gdk_window_end_draw_frame(window, drawingContext);

    cairo_region_destroy(cairoRegion);

    return FALSE;

}



int main(int argc, char* argv[])
{
    gtk_init(&argc, &argv);

    GdkRectangle workarea = { 0 };
    gdk_monitor_get_workarea(gdk_display_get_primary_monitor(gdk_display_get_default()), &workarea);

    y = workarea.height;
    x = 30;
    max_x = workarea.width;

    GtkWindow* window;
    window = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(window, 30, 140);
    gtk_window_set_decorated(window, false);
    g_signal_connect(window, "destroy", gtk_main_quit, NULL);
    gtk_widget_set_app_paintable((GtkWidget*)window, TRUE);
       
    GtkDrawingArea* drawingArea;
    drawingArea = (GtkDrawingArea*)gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), (GtkWidget*)drawingArea);
    g_signal_connect((GtkWidget*)drawingArea, "draw", G_CALLBACK(on_draw), NULL);


    gtk_widget_show_all((GtkWidget*)window);

    gtk_window_maximize(GTK_WINDOW(window));
    gtk_window_set_keep_above((GtkWindow*)window, true);




    gtk_main();

    return 0;
}