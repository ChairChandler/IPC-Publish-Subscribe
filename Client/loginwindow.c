#include "loginwindow.h"
#include "connect.h"
#include "msgbox.h"
#include <string.h>

#define INT_DIGITS 10

static ClientInfo exitClientInfo = {.clientFileDesc= -1, .serverFileDesc= -1, .lastError=NULL};
static SenderInfo exitSenderInfo = {.ID=0, .name=""};

gboolean loginwindow_logInOutSwitch(GtkSwitch *widget,
                                gboolean   state,
                                gpointer   user_data)
{
    GList *list;

    GtkWidget *window;
    GtkWidget *IPEntry;
    GtkWidget *nameEntry;
    GtkWidget *IDEntry;
    GtkWidget *messageEntry;
    GtkWidget *priorityEntry;
    GtkWidget *msgTypeComboBoxText;
    GtkWidget *newTypeEntry;

    list=user_data;



    GList *iter=list;

    window=iter->data;
    iter=iter->next;

    IPEntry=iter->data;
    iter=iter->next;

    nameEntry=iter->data;
    iter=iter->next;

    IDEntry=iter->data;
    iter=iter->next;

    messageEntry=iter->data;
    iter=iter->next;

    priorityEntry=iter->data;
    iter=iter->next;

    msgTypeComboBoxText=iter->data;
    iter=iter->next;

    newTypeEntry=iter->data;






    if(state == FALSE)
    {

        if(!strlen(gtk_entry_get_text(GTK_ENTRY(nameEntry))))
        {
            msgbox_error(window, "No name");
        }
        else if(!strlen(gtk_entry_get_text(GTK_ENTRY(IPEntry))))
        {
            msgbox_error(window, "No IP address");
        }
        else if(!strlen(gtk_entry_get_text(GTK_ENTRY(IDEntry))))
        {
            msgbox_error(window, "No sender ID");
        }
        else
        {
            strcpy(exitSenderInfo.name, gtk_entry_get_text(GTK_ENTRY(nameEntry)));

            exitSenderInfo.name[BUFF_SIZE-1]='\0';
            exitSenderInfo.ID=atoi(gtk_entry_get_text(GTK_ENTRY(IDEntry)));




            client_logout(&exitClientInfo, &exitSenderInfo);
            client_clearError(&exitClientInfo);

            if(client_isError(&exitClientInfo) == 0 || client_isServerActivity(&exitClientInfo) == -1)
            {
                msgbox_error(window, "Logout error");
            }
            else
            {
                client_exit(&exitClientInfo);
                gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(msgTypeComboBoxText));
                msgbox_success(window, "Logged out!");
            }
        }
    }
    else
    {



        if(!strlen(gtk_entry_get_text(GTK_ENTRY(nameEntry))))
        {
            msgbox_error(window, "No name");
        }
        else if(!strlen(gtk_entry_get_text(GTK_ENTRY(IPEntry))))
        {
            msgbox_error(window, "No IP address");
        }
        else if(!strlen(gtk_entry_get_text(GTK_ENTRY(IDEntry))))
        {
            msgbox_error(window, "No sender ID");
        }
        else
        {

            client_init(&exitClientInfo, gtk_entry_get_text(GTK_ENTRY(nameEntry)), atoi(gtk_entry_get_text(GTK_ENTRY(IPEntry))));



            strcpy(exitSenderInfo.name, gtk_entry_get_text(GTK_ENTRY(nameEntry)));



            exitSenderInfo.name[BUFF_SIZE-1]='\0';
            exitSenderInfo.ID=atoi(gtk_entry_get_text(GTK_ENTRY(IDEntry)));





            if(client_login(&exitClientInfo, &exitSenderInfo) == -1 || client_isServerActivity(&exitClientInfo) == -1)
            {
                msgbox_error(window, "Login error");
                client_clearError(&exitClientInfo);
            }
            else
            {
                msgbox_success(window, "Logged in!");



                char tmp[INT_DIGITS+1];
                MsgType *types;

                types=client_getTypes(&exitClientInfo, NULL, &exitSenderInfo);

                if(types == NULL)
                {
                    msgbox_error(window, "Receiving types error");
                    client_clearError(&exitClientInfo);
                    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(msgTypeComboBoxText));
                }
                else
                {
                    gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(msgTypeComboBoxText));

                    for(int i=0; i<BUFF_SIZE; i++)
                    {
                        if(types[i] != EmptyMsgType)
                        {
                            sprintf(tmp, "%zu", types[i]);
                            gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(msgTypeComboBoxText), tmp);
                        }
                    }
                }
            }
        }
    }

    return TRUE;
}

void loginwindow_sendMsg(GtkButton *button,
                   gpointer   user_data)
{
    GList *list;

    GtkWidget *window;
    GtkWidget *IPEntry;
    GtkWidget *nameEntry;
    GtkWidget *IDEntry;
    GtkWidget *messageEntry;
    GtkWidget *priorityEntry;
    GtkWidget *msgTypeComboBoxText;
    GtkWidget *newTypeEntry;

    list=user_data;


    GList *iter=list;

    window=iter->data;
    iter=iter->next;

    IPEntry=iter->data;
    iter=iter->next;

    nameEntry=iter->data;
    iter=iter->next;

    IDEntry=iter->data;
    iter=iter->next;

    messageEntry=iter->data;
    iter=iter->next;

    priorityEntry=iter->data;
    iter=iter->next;

    msgTypeComboBoxText=iter->data;
    iter=iter->next;

    newTypeEntry=iter->data;



    Message msg;

    if(!strlen(gtk_entry_get_text(GTK_ENTRY(nameEntry))))
    {
        msgbox_error(window, "No name");
    }
    else if(!strlen(gtk_entry_get_text(GTK_ENTRY(IPEntry))))
    {
        msgbox_error(window, "No IP address");
    }
    else if(!strlen(gtk_entry_get_text(GTK_ENTRY(IDEntry))))
    {
        msgbox_error(window, "No sender ID");
    }
    else if(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(msgTypeComboBoxText)) == NULL ||
                                               !strlen(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(msgTypeComboBoxText))))
    {
        msgbox_error(window, "No message type");
    }
    else if(!strlen(gtk_entry_get_text(GTK_ENTRY(priorityEntry))))
    {
        msgbox_error(window, "No message priority");
    }
    else if(!strlen(gtk_entry_get_text(GTK_ENTRY(messageEntry))))
    {
        msgbox_error(window, "No message");
    }
    else
    {
        client_init(&exitClientInfo, gtk_entry_get_text(GTK_ENTRY(nameEntry)), atoi(gtk_entry_get_text(GTK_ENTRY(IPEntry))));

        strcpy(exitSenderInfo.name, gtk_entry_get_text(GTK_ENTRY(nameEntry)));
        exitSenderInfo.name[BUFF_SIZE-1]='\0';
        exitSenderInfo.ID=atoi(gtk_entry_get_text(GTK_ENTRY(IDEntry)));

        msg.msgType=(MsgType)atoi(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(msgTypeComboBoxText)));

        msg.priority=(size_t)atoi(gtk_entry_get_text(GTK_ENTRY(priorityEntry)));

        strcpy(msg.text, gtk_entry_get_text(GTK_ENTRY(messageEntry)));
        msg.text[BUFF_SIZE-1]='\0';

        if(client_sendMsg(&exitClientInfo, &exitSenderInfo, &msg) == -1 ||  client_isServerActivity(&exitClientInfo) == -1)
        {
            msgbox_error(window, "Message sending error");
            client_clearError(&exitClientInfo);
        }
        else msgbox_success(window, "Message sended!");
    }
}

void loginwindow_newType(GtkButton *button,
                   gpointer   user_data)
{
    GList *list;

    GtkWidget *window;
    GtkWidget *IPEntry;
    GtkWidget *nameEntry;
    GtkWidget *IDEntry;
    GtkWidget *messageEntry;
    GtkWidget *priorityEntry;
    GtkWidget *msgTypeComboBoxText;
    GtkWidget *newTypeEntry;

    list=user_data;


    GList *iter=list;

    window=iter->data;
    iter=iter->next;

    IPEntry=iter->data;
    iter=iter->next;

    nameEntry=iter->data;
    iter=iter->next;

    IDEntry=iter->data;
    iter=iter->next;

    messageEntry=iter->data;
    iter=iter->next;

    priorityEntry=iter->data;
    iter=iter->next;

    msgTypeComboBoxText=iter->data;
    iter=iter->next;

    newTypeEntry=iter->data;




    if(!strlen(gtk_entry_get_text(GTK_ENTRY(nameEntry))))
    {
        msgbox_error(window, "No name");
    }
    else if(!strlen(gtk_entry_get_text(GTK_ENTRY(IPEntry))))
    {
        msgbox_error(window, "No IP address");
    }
    else if(!strlen(gtk_entry_get_text(GTK_ENTRY(IDEntry))))
    {
        msgbox_error(window, "No sender ID");
    }
    else
    {
        client_init(&exitClientInfo, gtk_entry_get_text(GTK_ENTRY(nameEntry)), atoi(gtk_entry_get_text(GTK_ENTRY(IPEntry))));

        strcpy(exitSenderInfo.name, gtk_entry_get_text(GTK_ENTRY(nameEntry)));

        exitSenderInfo.name[BUFF_SIZE-1]='\0';
        exitSenderInfo.ID=atoi(gtk_entry_get_text(GTK_ENTRY(IDEntry)));


        if(!strlen(gtk_entry_get_text(GTK_ENTRY(newTypeEntry))))
        {
            MsgType *msgType;
            msgType=client_randomType(&exitClientInfo, &exitSenderInfo);

            if(msgType == NULL)
            {
                msgbox_error(window, "Random new type creating error");
                client_clearError(&exitClientInfo);
            }
            else
            {
                char tmp[BUFF_SIZE];
                sprintf(tmp, "%zu", *msgType);

                gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(msgTypeComboBoxText), tmp);
                msgbox_success(window, "Random new type created!");

                free(msgType);
            }
        }
        else
        {
            MsgType msgType;
            msgType=atoi(gtk_entry_get_text(GTK_ENTRY(newTypeEntry)));

            if(client_newType(&exitClientInfo, &exitSenderInfo, &msgType) == -1 ||  client_isServerActivity(&exitClientInfo) == -1)
            {
                msgbox_error(window, "New type creating error");
                client_clearError(&exitClientInfo);
            }
            else
            {
                gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(msgTypeComboBoxText), NULL, gtk_entry_get_text(GTK_ENTRY(newTypeEntry)));
                msgbox_success(window, "New type created!");
            }
        }
    }
}

void loginwindow_getTypes(GtkButton *button,
                   gpointer   user_data)
{
    GList *list;

    GtkWidget *window;
    GtkWidget *IPEntry;
    GtkWidget *nameEntry;
    GtkWidget *IDEntry;
    GtkWidget *messageEntry;
    GtkWidget *priorityEntry;
    GtkWidget *msgTypeComboBoxText;
    GtkWidget *newTypeEntry;

    list=user_data;



    GList *iter=list;

    window=iter->data;
    iter=iter->next;

    IPEntry=iter->data;
    iter=iter->next;

    nameEntry=iter->data;
    iter=iter->next;

    IDEntry=iter->data;
    iter=iter->next;

    messageEntry=iter->data;
    iter=iter->next;

    priorityEntry=iter->data;
    iter=iter->next;

    msgTypeComboBoxText=iter->data;
    iter=iter->next;

    newTypeEntry=iter->data;








    if(!strlen(gtk_entry_get_text(GTK_ENTRY(nameEntry))))
    {
        msgbox_error(window, "No name");
    }
    else if(!strlen(gtk_entry_get_text(GTK_ENTRY(IPEntry))))
    {
        msgbox_error(window, "No IP address");
    }
    else if(!strlen(gtk_entry_get_text(GTK_ENTRY(IDEntry))))
    {
        msgbox_error(window, "No sender ID");
    }
    else
    {
        client_init(&exitClientInfo, gtk_entry_get_text(GTK_ENTRY(nameEntry)), atoi(gtk_entry_get_text(GTK_ENTRY(IPEntry))));

        strcpy(exitSenderInfo.name, gtk_entry_get_text(GTK_ENTRY(nameEntry)));

        exitSenderInfo.name[BUFF_SIZE-1]='\0';
        exitSenderInfo.ID=atoi(gtk_entry_get_text(GTK_ENTRY(IDEntry)));




        char tmp[BUFF_SIZE];
        MsgType *types;
        types=client_getTypes(&exitClientInfo, NULL, &exitSenderInfo);


        if(types == NULL)
        {
            msgbox_error(window, "Receiving types error");
            client_clearError(&exitClientInfo);
            gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(msgTypeComboBoxText));
        }
        else
        {
            gtk_combo_box_text_remove_all(GTK_COMBO_BOX_TEXT(msgTypeComboBoxText));

            for(int i=0; i<BUFF_SIZE; i++)
            {
                if(types[i] != EmptyMsgType)
                {
                    sprintf(tmp, "%zu", types[i]);
                    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(msgTypeComboBoxText), tmp);
                }
            }
        }
    }
}
