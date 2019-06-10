#include "listening.h"
#include "connect.h"
#include "msgbox.h"
#include <gtk/gtk.h>

ListeningThreadsInfo listeningThreads = {.notificationThread = 0,
                                        .messageThread = 0,
                                        .stopNotThread = False,
                                        .stopMessageThread = False};


/*
 * For some reason concluded from multithreading GUI,
 * message window appears only once.
 * GTK doesn't support threading/forks GUI correctly.
 */

static void showMsg(char msg[])
{
    GtkWidget *window;
    GtkWidget *txtLabel;
    GtkWidget *box;

    window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
    txtLabel=gtk_label_new(msg);
    box=gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    gtk_container_set_border_width (GTK_CONTAINER (window), 20);
    gtk_box_pack_start(GTK_BOX(box), txtLabel, TRUE, FALSE, 10);
    gtk_container_add(GTK_CONTAINER(window), box);

    gtk_widget_show_all(window);

    g_signal_connect(G_OBJECT(window), "activate-focus",
          G_CALLBACK(gtk_widget_destroy), window);

    gtk_main();
}

int listening_notification(void *arg)
{
    ClientInfo *client;
    client=arg;


    while(listeningThreads.stopNotThread == False)
    {
        if(client_isNotification(client) == 0)
        {
            showMsg("Waiting new msg!");
        }
    }

    listeningThreads.stopNotThread=False;
    return 0;
}

int listening_message(void *arg)
{
    ClientInfo *client;
    client=arg;

    Message *msg;

    while(listeningThreads.stopMessageThread == False)
    {
        if((msg=client_getAsyncMsg(client)) != NULL)
        {
            char tmp[BUFF_SIZE+20];

            sprintf(tmp, "New message:\n%s", msg->text);
            showMsg(tmp);
        }
    }

    listeningThreads.stopMessageThread=False;
    return 0;
}
