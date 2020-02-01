#include <gtk/gtk.h>

int main(int argc, char* argv[])
{
    GtkWidget* window;
    GtkWidget* layout;
    GtkWidget* image;

    gtk_init(&argc, &argv);

    GdkRectangle workarea = { 0 };
    gdk_monitor_get_workarea(gdk_display_get_primary_monitor(gdk_display_get_default()), &workarea);
    
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), workarea.width, workarea.height);
    //gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    //gtk_window_fullscreen(GTK_WINDOW(window));

    layout = gtk_layout_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(window), layout);
    gtk_widget_show(layout);

    image = gtk_image_new_from_file("Images/petester.png");
    gtk_layout_put(GTK_LAYOUT(layout), image, 0, 0);


    g_signal_connect_swapped(G_OBJECT(window), "destroy",
    G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}