#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include <gtk/gtk.h>
#include "client.h"
#include "senders.h"

gboolean loginwindow_logInOutSwitch(GtkSwitch *widget,
                                gboolean   state,
                                gpointer   user_data);

void loginwindow_sendMsg(GtkButton *button,
                   gpointer   user_data);

void loginwindow_newType(GtkButton *button,
                   gpointer   user_data);

void loginwindow_getTypes(GtkButton *button,
                   gpointer   user_data);

#endif // LOGINWINDOW_H
