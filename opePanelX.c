#include "commonfunction.h"
#include <gtk/gtk.h>

int current_floor, floor_number;
pid_t *list_pid;

GtkWidget *window;
GtkWidget *call_btn;
GtkWidget *close_btn;
GtkWidget *button_box;
GtkWidget *main_box;
GtkWidget *up_down_btn;
GtkWidget *current_floor_btn;
char window_title[10];

GdkPixbuf *create_pixbuf(const gchar *filename){
   	GdkPixbuf *pixbuf;
   	GError *error = NULL;
   	pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   	if (!pixbuf) {
      	fprintf(stderr, "%s\n", error->message);
      	g_error_free(error);
   	}
   	return pixbuf;
}

static void start_run(GtkWidget *widget, gpointer data){
	if(strcmp(gtk_widget_get_name(call_btn),"red_btn")!=0){
        gtk_widget_set_name(call_btn, "red_btn");       
        send_signal(list_pid[LIFT_MNG], (current_floor - 1) + SIGRTMIN + F1_CALL);    
    }
}

// xu ly thong bao vi tri hien tai cua thang may
void lift_current_position(int sigNo){
	if(floor_number > sigNo - SIGRTMIN - F1_ARRIVAL + 1){
        gtk_widget_set_name(current_floor_btn, "unready_btn");        
    }
	floor_number = sigNo - SIGRTMIN - F1_ARRIVAL + 1;
	switch (sigNo - SIGRTMIN){
		case F1_ARRIVAL:            
            gtk_button_set_label(GTK_BUTTON(current_floor_btn),"1");            
            break;
        case F2_ARRIVAL:            
            gtk_button_set_label(GTK_BUTTON(current_floor_btn),"2");
            break;
        case F3_ARRIVAL:            
            gtk_button_set_label(GTK_BUTTON(current_floor_btn),"3");
            break;
        case F4_ARRIVAL:            
            gtk_button_set_label(GTK_BUTTON(current_floor_btn),"4");
            break;
        case F5_ARRIVAL:            
            gtk_button_set_label(GTK_BUTTON(current_floor_btn),"5");
            break;  
        default:            
            break;
	}
}

// xu ly khi thang may dung lai
void lift_stop_move(){
	gtk_widget_set_name(call_btn, "default_btn");
}

// xu ly khi thang may dang duoc su dung
void lift_is_using(){
	gtk_widget_set_name(current_floor_btn, "ready_btn");
}

// khoi tao cac signal den cac tang, thang may dang dung, thang may dang su dung
void init_signal(){
	signal(SIGRTMIN + F1_ARRIVAL, lift_current_position);
	signal(SIGRTMIN + F2_ARRIVAL, lift_current_position);
	signal(SIGRTMIN + F3_ARRIVAL, lift_current_position);
	signal(SIGRTMIN + F4_ARRIVAL, lift_current_position);
	signal(SIGRTMIN + F5_ARRIVAL, lift_current_position);
	signal(SIGRTMIN + FINISHED, lift_stop_move);
	signal(SIGRTMIN + USING, lift_is_using);
}

static void activate(GtkApplication *app,gpointer user_data){
    
    // <-------------  For add custom.css
    GtkCssProvider* provider = gtk_css_provider_new();
    GdkDisplay* display = gdk_display_get_default();
    GdkScreen* screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(provider), "custom.css", NULL);
    
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), window_title);
    gtk_window_set_default_size(GTK_WINDOW(window), 150, 100);
        
    gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("icon"));
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    //   Add Vbox
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    button_box = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
    gtk_container_add(GTK_CONTAINER(main_box), button_box);

    switch(current_floor){
        case 5:
            gtk_window_move(GTK_WINDOW(window),50,50);
            up_down_btn = gtk_button_new_with_label("FLOOR 5");
        	break;
        case 4:
            up_down_btn = gtk_button_new_with_label("FLOOR 4");
            gtk_window_move(GTK_WINDOW(window),50,160);
        	break;
        case 3:
            up_down_btn = gtk_button_new_with_label("FLOOR 3");
            gtk_window_move(GTK_WINDOW(window),50,270);
        	break;
        case 2:
            up_down_btn = gtk_button_new_with_label("FLOOR 2");
            gtk_window_move(GTK_WINDOW(window),50,380);
        	break;
        default:
        	break;
    }
    gtk_widget_set_name(up_down_btn, "info_btn");
    gtk_widget_set_sensitive(up_down_btn, FALSE);
    gtk_container_add(GTK_CONTAINER(button_box), up_down_btn);

    current_floor_btn = gtk_button_new_with_label("1");
    gtk_widget_set_name(current_floor_btn, "unready_btn");

    gtk_widget_set_sensitive(current_floor_btn, FALSE);
    gtk_container_add(GTK_CONTAINER(button_box), current_floor_btn);
    // button x
    call_btn = gtk_button_new_with_label("CALL");
    g_signal_connect(call_btn, "clicked", G_CALLBACK(start_run),NULL);
    gtk_container_add(GTK_CONTAINER(button_box), call_btn);
    
    // show all widget
    gtk_widget_show_all(window);
}

int main(int argc, char const *argv[])
{
	GtkApplication *app;
    int status;
	init_signal();
	if (argc != 2)
	{
		printf("Usage: opx NUMBER FLOOR\n");
		exit(0);
	}
	current_floor = atoi(argv[1]);
	int signal_floor;
	switch (current_floor){
		case 2:
			list_pid = create_pid(OPE_PANEL2);
			setpgid(list_pid[OPE_PANEL2], list_pid[LIFT_MNG]);
			signal_floor = SIGRTMIN + F2_CALL;
			strcpy(window_title,"Tầng 2");
			app = gtk_application_new("org.gtk.tang2", G_APPLICATION_FLAGS_NONE);
			break;
		case 3: 
			list_pid = create_pid(OPE_PANEL3);
			setpgid(list_pid[OPE_PANEL3], list_pid[LIFT_MNG]);
			signal_floor = SIGRTMIN + F3_CALL;
			strcpy(window_title,"Tầng 3");
			app = gtk_application_new("org.gtk.tang3", G_APPLICATION_FLAGS_NONE);
			break;
		case 4:
			list_pid = create_pid(OPE_PANEL4);
			setpgid(list_pid[OPE_PANEL4], list_pid[LIFT_MNG]);
			signal_floor = SIGRTMIN + F4_CALL;
			strcpy(window_title,"Tầng 4");
			app = gtk_application_new("org.gtk.tang4", G_APPLICATION_FLAGS_NONE);
			break;
		case 5:
			list_pid = create_pid(OPE_PANEL5);
			setpgid(list_pid[OPE_PANEL5], list_pid[LIFT_MNG]);
			signal_floor = SIGRTMIN + F5_CALL;
			strcpy(window_title,"Tầng 5");
			app = gtk_application_new("org.gtk.tang5", G_APPLICATION_FLAGS_NONE);
			break;
		default:
			break;
	}
	g_signal_connect(app, "activate", G_CALLBACK(activate), &signal_floor);
    status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);
	return status;
}