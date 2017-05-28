#include "commonfunction.h"
#include <gtk/gtk.h>

pid_t *list_pid;
int current_floor = 1;
int fifoFd;
GtkWidget *window;
GtkWidget *button2;
GtkWidget *button3;
GtkWidget *button4;
GtkWidget *button5;
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

// xu ly thong bao vi tri hien tai cua thang may
void lift_current_position(int sigNo){
	gtk_widget_set_name(current_floor_btn, "unready_btn");
    current_floor = sigNo - SIGRTMIN;
	switch(current_floor){
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
	int floor;
	if (read(fifoFd, &floor, sizeof(int)) > 0)
	{
		switch (floor){
			case 2:
				gtk_widget_set_name(button2, "default_btn");
				break;
			case 3:
				gtk_widget_set_name(button3, "default_btn");
				break;
			case 4:
				gtk_widget_set_name(button4, "default_btn");
				break;
			case 5:
				gtk_widget_set_name(button5, "default_btn");
				break;
			default:
				break;
		}
	}
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

// nhan request chuyen sang process mng de thuc hien
static void request_floor2(GtkWidget *widget, gpointer data){
	int floor = 2;
	if(strcmp(gtk_widget_get_name(button2), "red_btn") != 0){
        gtk_widget_set_name(button2, "red_btn");
        send_signal(list_pid[LIFT_MNG], SIGRTMIN + F2_UP);
		write(fifoFd, &floor, sizeof(int)); 
    }
}

static void request_floor3(GtkWidget *widget, gpointer data){
	int floor = 3;
	if(strcmp(gtk_widget_get_name(button3), "red_btn") != 0){
        gtk_widget_set_name(button3, "red_btn");
        send_signal(list_pid[LIFT_MNG], SIGRTMIN + F3_UP);
		write(fifoFd, &floor, sizeof(int)); 
    }
}

static void request_floor4(GtkWidget *widget, gpointer data){
	int floor = 4;
	if(strcmp(gtk_widget_get_name(button4), "red_btn") != 0){
        gtk_widget_set_name(button4, "red_btn");
        send_signal(list_pid[LIFT_MNG], SIGRTMIN + F4_UP);
		write(fifoFd, &floor, sizeof(int));
    }	
}

static void request_floor5(GtkWidget *widget, gpointer data){
	int floor = 5;
	if(strcmp(gtk_widget_get_name(button5), "red_btn") != 0){
        gtk_widget_set_name(button5, "red_btn");
        send_signal(list_pid[LIFT_MNG], SIGRTMIN + F5_UP);
		write(fifoFd, &floor, sizeof(int));
    }
}

static void exit_app(){
	send_signal(list_pid[LIFT_MNG], SIGINT);
	send_signal(list_pid[LIFT_CTR], SIGINT);
	pid_t this_pid = getpid();
	for (int i = OPE_PANEL1; i <= OPE_PANEL5; i++)
	{
		if (list_pid[i] != this_pid)
		{
			send_signal(list_pid[i], SIGINT);
		}
	}
	shmdt(list_pid);
	release_shm();
	gtk_widget_destroy(window);
}

static void activate(GtkApplication *app, gpointer user_data){
    // For add custom.css
    GtkCssProvider* provider = gtk_css_provider_new();
    GdkDisplay* display = gdk_display_get_default();
    GdkScreen* screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(provider), "custom.css", NULL);

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), window_title);
    gtk_window_set_default_size(GTK_WINDOW(window), 150, 200);
    gtk_window_move(GTK_WINDOW(window),50,490);
    gtk_window_set_icon(GTK_WINDOW(window), create_pixbuf("icon"));
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);

    //   Add Vbox
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    button_box = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
    gtk_container_add(GTK_CONTAINER(main_box), button_box);

    up_down_btn = gtk_button_new_with_label("FLOOR 1");
    gtk_widget_set_name(up_down_btn, "info_btn");
    gtk_widget_set_sensitive(up_down_btn, FALSE);
    gtk_container_add(GTK_CONTAINER(button_box), up_down_btn);

    current_floor_btn = gtk_button_new_with_label("1");
    gtk_widget_set_name(current_floor_btn, "unready_btn");    

    gtk_widget_set_sensitive(current_floor_btn, FALSE);
    gtk_container_add(GTK_CONTAINER(button_box), current_floor_btn);

    // button 2
    button2 = gtk_button_new_with_label("2");
    g_signal_connect(button2, "clicked", G_CALLBACK(request_floor2), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), button2);

    // button3
    button3 = gtk_button_new_with_label("3");
    g_signal_connect(button3, "clicked", G_CALLBACK(request_floor3), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), button3);

    // button4
    button4 = gtk_button_new_with_label("4");
    g_signal_connect(button4, "clicked", G_CALLBACK(request_floor4), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), button4);

    // button5
    button5 = gtk_button_new_with_label("5");
    g_signal_connect(button5, "clicked", G_CALLBACK(request_floor5), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), button5);

     // close_btn
    close_btn = gtk_button_new_with_label("Close");
    g_signal_connect(close_btn, "clicked", G_CALLBACK(exit_app), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), close_btn);

    // show all widget
    gtk_widget_show_all(window);
}

int main(int argc, char const *argv[])
{
	init_signal();
	list_pid = create_pid(OPE_PANEL1);
	setpgid(list_pid[OPE_PANEL1], list_pid[LIFT_MNG]);

	// create queue fifo file
	remove(OPE1_FIFO_FILE_PATH);
	if (mkfifo(OPE1_FIFO_FILE_PATH,0666) == -1){
        perror("mkfifo");
        exit(1);
    }

    if ((fifoFd=open(OPE1_FIFO_FILE_PATH,O_RDWR|O_NONBLOCK)) == -1){
        perror("fifofile open");
        exit(1);
    }

    GtkApplication *app;
    int status;
    strcpy(window_title,"Tầng 1");    
    app = gtk_application_new("org.gtk.tang1", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);

    close(fifoFd);
    unlink(OPE1_FIFO_FILE_PATH);
	return status;
}