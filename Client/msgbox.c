#include "msgbox.h"

void msgbox_error(GtkWidget *window, const char error[])
{
    GtkWidget *msgBox;

    msgBox=gtk_message_dialog_new(GTK_WINDOW(window),
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_ERROR,
                                  GTK_BUTTONS_CLOSE,
                                  "%s", error);

    gtk_dialog_run(GTK_DIALOG(msgBox));
    gtk_widget_destroy(msgBox);
}

void msgbox_success(GtkWidget *window, const char info[])
{
    GtkWidget *msgBox;

    msgBox=gtk_message_dialog_new(GTK_WINDOW(window),
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_INFO,
                                  GTK_BUTTONS_CLOSE,
                                  "%s", info);

    gtk_dialog_run(GTK_DIALOG(msgBox));
    gtk_widget_destroy(msgBox);
}
