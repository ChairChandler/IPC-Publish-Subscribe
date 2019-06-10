#include "removewidgets.h"

void removeWidgets(GtkWidget *window)
{
    GList *list, *first, *last;

    list=gtk_container_get_children(GTK_CONTAINER(window));

    first=g_list_first(list);
    last=g_list_last(list);

    for(GList *iter=list; iter!=NULL; iter=g_list_next(list))
    {
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    }

    list=g_list_delete_link(first, last);
}
