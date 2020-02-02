#include <gtk/gtk.h>
#include <cairo.h>
#include <stdlib.h> 
#include <string>

int x;
int max_x;
int y;

int dirx = 0;
int target_x = 0;

const char* right[] = { "Images/4.png", "Images/5.png", "Images/6.png" };
const char* left[] = { "Images/10.png", "Images/11.png", "Images/12.png" };
int img_pos = -1;
int img_pos_adder = 1;

int animation_counter = 0;

bool text_showing = false;
int text_counter = 200;
const char* text[] = { "u got this", "so proud of u", "love you hehe", "ur so cool", "uwu", "dont give up", "owo", "qq keep going", "keep fighting", "ur amazing", "u work so hard", "stay strong", "focus!" };
int text_n = 13;
int text_num;


gboolean on_draw(GtkWidget* widget, GdkEventExpose* event, gpointer data)
{
    GdkWindow* window = gtk_widget_get_window(widget);
    cairo_region_t* cairoRegion = cairo_region_create();
    GdkDrawingContext* drawingContext;
    drawingContext = gdk_window_begin_draw_frame(window, cairoRegion);

    cairo_t* cr = gdk_drawing_context_get_cairo_context(drawingContext);

    if (text_showing) {
        cairo_set_source_rgb(cr, 230, 230, 230);
        cairo_select_font_face(cr, "Purisa", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 13);

        int y_loc = y - 55;
        int x_loc = x * 2;

        cairo_move_to(cr, x_loc, y_loc);
        cairo_show_text(cr, text[text_num]);
    }
    int              w, h;
    cairo_surface_t* image;

    char filename[15];
    
    if (dirx == 0) {
        strcpy_s(filename, "Images/8.png");
    }
    else{
        if (img_pos == -1) {
            img_pos = 1;
        }

        if (dirx > 0) strcpy_s(filename, right[img_pos]);
        else strcpy_s(filename, left[img_pos]);

        if (animation_counter == 5) {
            if (img_pos + img_pos_adder >= 3) {
                img_pos_adder = -1;
            }
            if (img_pos + img_pos_adder < 0) {
                img_pos_adder = 1;
            }
            img_pos = img_pos + img_pos_adder;
            animation_counter = 0;
        }
        else {
            animation_counter++;
        }
    }

    image = cairo_image_surface_create_from_png(filename);
    w = cairo_image_surface_get_width(image);
    h = cairo_image_surface_get_height(image);

    cairo_scale(cr, 2, 2);

    int y_pos = y / 2 - h;

    x = x + dirx;

    if (x == target_x) {
        dirx = 0;
        img_pos = -1;
    }

    cairo_set_source_surface(cr, image, x, y_pos);
    cairo_paint(cr);
    cairo_surface_destroy(image);

    gdk_window_end_draw_frame(window, drawingContext);

    cairo_region_destroy(cairoRegion);

    return FALSE;

}

static gboolean time_handler(GtkWidget* widget)
{
    if ((GtkWindow*)widget == NULL) return FALSE;

    time_t curtime;
    struct tm loctime;

    curtime = time(NULL);
    localtime_s(&loctime, &curtime);

    if (dirx == 0) {
        if (rand() % 150 == 0) {
            target_x = rand() % (max_x / 2);
            if (target_x > x) {
                dirx = 1;
            }
            else if (target_x < x) {
                dirx = -1;
            }
        }
    }

    if (text_showing == false) {
        if (rand() % 200 == 0) {
            text_showing = true;
            text_counter = 200;
            text_num = rand() % text_n;
        }
    }
    else {
        text_counter--;
        if (text_counter == 0) {
            text_showing = false;
        }
    }

    gtk_widget_queue_draw(widget);
    return TRUE;
}


int main(int argc, char* argv[])
{
    gtk_init(&argc, &argv);

    GdkRectangle workarea = { 0 };
    gdk_monitor_get_workarea(gdk_display_get_primary_monitor(gdk_display_get_default()), &workarea);

    y = workarea.height;
    x = 30;
    max_x = workarea.width - 25;

    GtkWindow* window;
    window = (GtkWindow*)gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(window, "i love you");
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


    g_timeout_add(33, (GSourceFunc)time_handler, (gpointer)window);
    time_handler((GtkWidget*)window);


    gtk_main();

    return 0;
}