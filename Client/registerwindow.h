#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H
#include <gtk/gtk.h>
#include "connect.h"


void registerwindow_register(GtkButton *button,
                   gpointer   user_data);

void registerwindow_unregister(GtkButton *button,
                   gpointer   user_data);

void registerwindow_getMsg(GtkButton *button,
                   gpointer   user_data);

void registerwindow_getTypes(GtkButton *button,
                   gpointer   user_data);

void registerwindow_addTypeToRcvArray(GtkButton *button,
                   gpointer   user_data);

void registerwindow_subTypeFromRcvArray(GtkButton *button,
                   gpointer   user_data);


#endif // REGISTERWINDOW_H
