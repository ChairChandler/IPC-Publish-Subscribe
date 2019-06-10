#include "registerwindow.h"
#include "removewidgets.h"
#include "msgbox.h"
#include "listening.h"

#define INT_DIGITS 10

static ReceiverInfo exitReceiverInfo = {.name="", .rcvMsgTypes={-1}};


void registerwindow_register(GtkButton *button,
                   gpointer   user_data)
{
    GList *list;
    GtkWidget *window;
    GtkWidget *IPEntry;
    GtkWidget *nameEntry;
    GtkWidget *msgTypeComboBoxText;

    ClientInfo *client;
    ReceiverInfo *receiver;




    list=user_data;

    window=list->data;
    list=list->next;

    IPEntry=list->data;
    list=list->next;

    nameEntry=list->data;
    list=list->next;

    msgTypeComboBoxText=list->data;
    list=list->next;

    client=list->data;
    list=list->next;

    receiver=list->data;






    if(!strlen(gtk_entry_get_text(GTK_ENTRY(nameEntry))))
    {
        msgbox_error(window, "No name");
    }
    else if(!strlen(gtk_entry_get_text(GTK_ENTRY(IPEntry))))
    {
        msgbox_error(window, "No IP address");
    }
    else
    {
        client_init(client, gtk_entry_get_text(GTK_ENTRY(nameEntry)), atoi(gtk_entry_get_text(GTK_ENTRY(IPEntry))));

        strcpy(receiver->name, gtk_entry_get_text(GTK_ENTRY(nameEntry)));
        receiver->name[BUFF_SIZE-1]='\0';



        if(client_register(client, receiver) == -1 ||  client_isServerActivity(client) == -1)
        {
            msgbox_error(window, "Register error");
        }
        else
        {
            msgbox_success(window, "Registered!");

            if(receiver->rcvNot == Notifications)
            {
                if(listeningThreads.notificationThread != 0)
                {
                    listeningThreads.stopNotThread=True;
                    while(listeningThreads.stopNotThread == True);
                }

                listeningThreads.notificationThread=g_idle_add(listening_notification, client);
            }


            if(receiver->rcvSend == Automatic)
            {
                if(listeningThreads.messageThread != 0)
                {
                    listeningThreads.stopMessageThread=True;
                    while(listeningThreads.stopMessageThread == True);
                }

                listeningThreads.messageThread=g_idle_add(listening_message, client);
            }
        }

        exitReceiverInfo=*receiver;
    }
}

void registerwindow_unregister(GtkButton *button,
                   gpointer   user_data)
{
    GList *list;
    GtkWidget *window;
    GtkWidget *IPEntry;
    GtkWidget *nameEntry;
    GtkWidget *msgTypeComboBoxText;

    ClientInfo *client;
    ReceiverInfo *receiver;




    list=user_data;

    window=list->data;
    list=list->next;

    IPEntry=list->data;
    list=list->next;

    nameEntry=list->data;
    list=list->next;

    msgTypeComboBoxText=list->data;
    list=list->next;

    client=list->data;
    list=list->next;

    receiver=list->data;






    if(!strlen(gtk_entry_get_text(GTK_ENTRY(nameEntry))))
    {
        msgbox_error(window, "No name");
    }
    else if(!strlen(gtk_entry_get_text(GTK_ENTRY(IPEntry))))
    {
        msgbox_error(window, "No IP address");
    }
    else
    {
        client_init(client, gtk_entry_get_text(GTK_ENTRY(nameEntry)), atoi(gtk_entry_get_text(GTK_ENTRY(IPEntry))));

        strcpy(receiver->name, gtk_entry_get_text(GTK_ENTRY(nameEntry)));
        receiver->name[BUFF_SIZE-1]='\0';


        if(client_unregister(client, receiver) == -1 ||  client_isServerActivity(client) == -1)
        {
            msgbox_error(window, "Unregister error");
        }
        else
        {
            msgbox_success(window, "Unregistered!");
        }

        exitReceiverInfo=*receiver;
    }
}

void registerwindow_getMsg(GtkButton *button,
                   gpointer   user_data)
{
    GList *list;

    GtkWidget *window;
    GtkWidget *IPEntry;
    GtkWidget *nameEntry;
    GtkWidget *msgTypeComboBoxText;
    ClientInfo *client;
    ReceiverInfo *receiver;




    list=user_data;

    window=list->data;
    list=list->next;

    IPEntry=list->data;
    list=list->next;

    nameEntry=list->data;
    list=list->next;

    msgTypeComboBoxText=list->data;
    list=list->next;

    client=list->data;
    list=list->next;

    receiver=list->data;



    if(!strlen(gtk_entry_get_text(GTK_ENTRY(nameEntry))))
    {
        msgbox_error(window, "No name");
    }
    else if(!strlen(gtk_entry_get_text(GTK_ENTRY(IPEntry))))
    {
        msgbox_error(window, "No IP address");
    }
    else
    {
        client_init(client, gtk_entry_get_text(GTK_ENTRY(nameEntry)), atoi(gtk_entry_get_text(GTK_ENTRY(IPEntry))));

        strcpy(receiver->name, gtk_entry_get_text(GTK_ENTRY(nameEntry)));
        receiver->name[BUFF_SIZE-1]='\0';

        Message *msg;
        msg=client_getMsg(client, receiver);

        if(msg == NULL)
        {
            msgbox_error(window, "Message not delivered properly");
            client_clearError(client);
        }
        else
        {
            char tmp[BUFF_SIZE+32];
            sprintf(tmp, "Message:\n %s", msg->text);
            msgbox_success(window, tmp);
        }

        exitReceiverInfo=*receiver;
    }
}


void registerwindow_getTypes(GtkButton *button,
                   gpointer   user_data)
{
    GList *list;

    GtkWidget *window;
    GtkWidget *IPEntry;
    GtkWidget *nameEntry;
    GtkWidget *msgTypeComboBoxText;
    ClientInfo *client;
    ReceiverInfo *receiver;




    list=user_data;

    window=list->data;
    list=list->next;

    IPEntry=list->data;
    list=list->next;

    nameEntry=list->data;
    list=list->next;

    msgTypeComboBoxText=list->data;
    list=list->next;

    client=list->data;
    list=list->next;

    receiver=list->data;






    if(!strlen(gtk_entry_get_text(GTK_ENTRY(nameEntry))))
    {
        msgbox_error(window, "No name");
    }
    else if(!strlen(gtk_entry_get_text(GTK_ENTRY(IPEntry))))
    {
        msgbox_error(window, "No IP address");
    }
    else
    {
        client_init(client, gtk_entry_get_text(GTK_ENTRY(nameEntry)), atoi(gtk_entry_get_text(GTK_ENTRY(IPEntry))));

        strcpy(receiver->name, gtk_entry_get_text(GTK_ENTRY(nameEntry)));

        receiver->name[BUFF_SIZE-1]='\0';




        char tmp[BUFF_SIZE];
        MsgType *types;
        types=client_getTypes(client, receiver, NULL);


        if(types == NULL)
        {
            msgbox_error(window, "Receiving types error");
            client_clearError(client);
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

void registerwindow_addTypeToRcvArray(GtkButton *button,
                   gpointer   user_data)
{
    GList *list;

    GtkWidget *window;
    GtkWidget *IPEntry;
    GtkWidget *nameEntry;
    GtkWidget *msgTypeComboBoxText;
    ClientInfo *client;
    ReceiverInfo *receiver;




    list=user_data;

    window=list->data;
    list=list->next;

    IPEntry=list->data;
    list=list->next;

    nameEntry=list->data;
    list=list->next;

    msgTypeComboBoxText=list->data;
    list=list->next;

    client=list->data;
    list=list->next;

    receiver=list->data;




    if(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(msgTypeComboBoxText)) == NULL ||
            !strlen(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(msgTypeComboBoxText))))
    {
        msgbox_error(window, "No message type");
    }
    else
    {
        MsgType userMsgType;
        Bool isEmpty;
        Bool isError;

        isEmpty=False;
        isError=False;
        userMsgType=(MsgType)atoi(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(msgTypeComboBoxText)));

        for(int i=0; i<BUFF_SIZE; i++)
        {
            if(receiver->rcvMsgTypes[i] == userMsgType)
            {
                isError=True;
                msgbox_error(window, "Duplicate message type");
                break;
            }
            else if(receiver->rcvMsgTypes[i] == EmptyMsgType)
            {
                isEmpty=True;
                receiver->rcvMsgTypes[i]=userMsgType;
                break;
            }
        }


        if(isEmpty == False && isError == False)
        {
            msgbox_error(window, "Full message type array");
        }
        else
        {
            exitReceiverInfo=*receiver;
        }
    }
}

void registerwindow_subTypeFromRcvArray(GtkButton *button,
                   gpointer   user_data)
{
    GList *list;

    GtkWidget *window;
    GtkWidget *IPEntry;
    GtkWidget *nameEntry;
    GtkWidget *msgTypeComboBoxText;
    ClientInfo *client;
    ReceiverInfo *receiver;




    list=user_data;

    window=list->data;
    list=list->next;

    IPEntry=list->data;
    list=list->next;

    nameEntry=list->data;
    list=list->next;

    msgTypeComboBoxText=list->data;
    list=list->next;

    client=list->data;
    list=list->next;

    receiver=list->data;




    if(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(msgTypeComboBoxText)) == NULL ||
            !strlen(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(msgTypeComboBoxText))))
    {
        msgbox_error(window, "No message type");
    }
    else
    {
        MsgType userMsgType;
        Bool isMsgTypeInArray;

        isMsgTypeInArray=False;
        userMsgType=(MsgType)atoi(gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(msgTypeComboBoxText)));

        for(int i=0; i<BUFF_SIZE; i++)
        {
            if(receiver->rcvMsgTypes[i] == userMsgType)
            {
                isMsgTypeInArray=True;
                receiver->rcvMsgTypes[i]=EmptyMsgType;
                break;
            }
        }


        if(isMsgTypeInArray == False)
        {
            msgbox_error(window, "Type not found in array");
        }
        else
        {
            exitReceiverInfo=*receiver;
        }
    }
}
