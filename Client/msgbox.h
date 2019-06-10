#ifndef MSGBOX_H
#define MSGBOX_H
#include <gtk/gtk.h>

void msgbox_error(GtkWidget *window, const char error[]);
void msgbox_success(GtkWidget *window, const char info[]);

#endif // MSGBOX_H
