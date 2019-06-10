#include <gtk/gtk.h>
#include "loginwindow.h"
#include "registerwindow.h"
#include "removewidgets.h"
#include "msgbox.h"
#include "listening.h"

#define INT_DIGITS 10
#define PADDING 5

static void menuWindowInit(GtkApplication *app,
                     gpointer user_data);

static void menuWindow(GtkButton *button,
                       gpointer   user_data);

static void loginButton(GtkButton *button,
                       gpointer   user_data);

static void registerButton(GtkButton *button,
                       gpointer   user_data);

static void exitButton(GtkButton *button,
                       gpointer   user_data);

static void optionsButton(GtkButton *button,
                       gpointer   user_data);

static gboolean options_notificationSwitch(GtkSwitch *widget,
                                           gboolean   state,
                                           gpointer   user_data);

static gboolean options_subscriptionSwitch(GtkSwitch *widget,
                                           gboolean   state,
                                           gpointer   user_data);

static gboolean options_sendSwitch(GtkSwitch *widget,
                                   gboolean   state,
                                   gpointer   user_data);

static void glist_omit(GList **glist, int number);





int main(int argc, char *argv[])
{
    GtkApplication *app;
    int status;
    int id;
    char appName[32];



    id=getpid();
    sprintf(appName, "client_%d.app",id);

    app=gtk_application_new(appName, G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(menuWindowInit), NULL);
    status=g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

static void menuWindowInit(GtkApplication *app,
                 gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *buttonLogin;
    GtkWidget *buttonRegister;
    GtkWidget *buttonExit;


    window=gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Client");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_container_set_border_width (GTK_CONTAINER (window), 20);


    box=gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);


    buttonLogin=gtk_button_new_with_label("Login");
    buttonRegister=gtk_button_new_with_label("Register");
    buttonExit=gtk_button_new_with_label("Exit");



    GList *list;

    list=NULL;
    list=g_list_append(list, window);



    g_signal_connect(buttonLogin, "clicked", G_CALLBACK(loginButton), list);
    g_signal_connect(buttonRegister, "clicked", G_CALLBACK(registerButton), list);
    g_signal_connect(buttonExit, "clicked", G_CALLBACK(exitButton), list);

    gtk_box_pack_start(GTK_BOX(box), buttonLogin, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), buttonRegister, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), buttonExit, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_widget_show_all(window);
}

static void menuWindow(GtkButton *button,
                       gpointer   user_data)
{
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *buttonLogin;
    GtkWidget *buttonRegister;
    GtkWidget *buttonExit;


    GList *list;
    list=user_data;


    window=list->data;
    removeWidgets(window);


    gtk_window_set_title(GTK_WINDOW(window), "Client");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_widget_set_size_request(window, 200, 100);
    gtk_container_set_border_width (GTK_CONTAINER (window), 20);


    box=gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);


    buttonLogin=gtk_button_new_with_label("Login");
    buttonRegister=gtk_button_new_with_label("Register");
    buttonExit=gtk_button_new_with_label("Exit");


    g_signal_connect(buttonLogin, "clicked", G_CALLBACK(loginButton), list);
    g_signal_connect(buttonRegister, "clicked", G_CALLBACK(registerButton), list);
    g_signal_connect(buttonExit, "clicked", G_CALLBACK(exitButton), list);


    gtk_box_pack_start(GTK_BOX(box), buttonLogin, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), buttonRegister, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), buttonExit, FALSE, FALSE, 0);


    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_widget_show_all(window);
}

static void loginButton(GtkButton *button,
                       gpointer   user_data)
{
    GtkWidget *window;

    GtkWidget *vbox;
    GtkWidget *hbox[10];

    GtkWidget *IPLabel;
    GtkWidget *nameLabel;
    GtkWidget *IDLabel;
    GtkWidget *messageLabel;
    GtkWidget *priorityLabel;
    GtkWidget *msgTypeLabel;
    GtkWidget *newTypeLabel;
    GtkWidget *logInOutLabel;

    GtkWidget *IPEntry;
    GtkWidget *nameEntry;
    GtkWidget *IDEntry;
    GtkWidget *messageEntry;
    GtkWidget *priorityEntry;
    GtkWidget *newTypeEntry;

    GtkWidget *msgTypeComboBoxText;

    GtkWidget *logInOutMsgSwitch;

    GtkWidget *newTypeButton;
    GtkWidget *sendMsgButton;
    GtkWidget *backButton;
    GtkWidget *getMsgTypeButton;

    GList *list;
    list=user_data;

    window=list->data;
    removeWidgets(window);

    gtk_window_resize(GTK_WINDOW(window), 250, 250);

    vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL, PADDING);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    for(int i=0; i<10; i++)
    {
        hbox[i]=gtk_box_new(GTK_ORIENTATION_HORIZONTAL, PADDING);
        gtk_box_pack_start(GTK_BOX(vbox), hbox[i], TRUE, FALSE, PADDING);
    }


    IPLabel=gtk_label_new("Server IP: ");
    nameLabel=gtk_label_new("Name: ");
    IDLabel=gtk_label_new("ID: ");
    messageLabel=gtk_label_new("Message: ");
    priorityLabel=gtk_label_new("Priority: ");
    msgTypeLabel=gtk_label_new("Message type: ");
    newTypeLabel=gtk_label_new("New type: ");
    logInOutLabel=gtk_label_new("Logout/Login");


    IPEntry=gtk_entry_new();
    nameEntry=gtk_entry_new();
    IDEntry=gtk_entry_new();
    messageEntry=gtk_entry_new();
    priorityEntry=gtk_entry_new();
    newTypeEntry=gtk_entry_new();


    gtk_entry_set_max_length(GTK_ENTRY(IPEntry), INT_DIGITS);
    gtk_entry_set_max_length(GTK_ENTRY(nameEntry), BUFF_SIZE-1);
    gtk_entry_set_max_length(GTK_ENTRY(IDEntry), INT_DIGITS);
    gtk_entry_set_max_length(GTK_ENTRY(messageEntry), BUFF_SIZE-1);
    gtk_entry_set_max_length(GTK_ENTRY(priorityEntry), INT_DIGITS);


    msgTypeComboBoxText=gtk_combo_box_text_new();


    logInOutMsgSwitch=gtk_switch_new();

    sendMsgButton=gtk_button_new_with_label("Send message");
    newTypeButton=gtk_button_new_with_label("New Type");
    backButton=gtk_button_new_with_label("Menu");
    getMsgTypeButton=gtk_button_new_with_label("Get\ntypes");

    gtk_box_pack_start(GTK_BOX(hbox[0]), IPLabel, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[0]), IPEntry, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[1]), nameLabel, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[1]), nameEntry, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[2]), IDLabel, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[2]), IDEntry, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[3]), messageLabel, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[3]), messageEntry, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[4]), priorityLabel, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[4]), priorityEntry, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[5]), msgTypeLabel, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[5]), msgTypeComboBoxText, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[5]), getMsgTypeButton, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[6]), newTypeLabel, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[6]), newTypeEntry, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[7]), logInOutLabel, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[7]), logInOutMsgSwitch, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[8]), sendMsgButton, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[8]), newTypeButton, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[9]), backButton, TRUE, FALSE, PADDING);


    GList *newList;

    newList=NULL;
    newList=g_list_append(newList, window);
    newList=g_list_append(newList, IPEntry);
    newList=g_list_append(newList, nameEntry);
    newList=g_list_append(newList, IDEntry);
    newList=g_list_append(newList, messageEntry);
    newList=g_list_append(newList, priorityEntry);
    newList=g_list_append(newList, msgTypeComboBoxText);
    newList=g_list_append(newList, newTypeEntry);


    g_signal_connect(logInOutMsgSwitch, "state-set", G_CALLBACK(loginwindow_logInOutSwitch), newList);
    g_signal_connect(sendMsgButton, "clicked", G_CALLBACK(loginwindow_sendMsg), newList);
    g_signal_connect(newTypeButton, "clicked", G_CALLBACK(loginwindow_newType), newList);
    g_signal_connect(backButton, "clicked", G_CALLBACK(menuWindow), newList);
    g_signal_connect(getMsgTypeButton, "clicked", G_CALLBACK(loginwindow_getTypes), newList);

    gtk_widget_show_all(window);
}

static void registerButton(GtkButton *button,
                       gpointer   user_data)
{
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *subRegVBox;
    GtkWidget *subvbox;
    GtkWidget *hbox[5];

    GtkWidget *IPLabel;
    GtkWidget *nameLabel;

    GtkWidget *IPEntry;
    GtkWidget *nameEntry;

    GtkWidget *msgTypeComboBoxText;

    GtkWidget *registerAccButton;
    GtkWidget *unregisterAccButton;
    GtkWidget *getMsgButton;
    GtkWidget *optsButton;
    GtkWidget *addTypeButton;
    GtkWidget *subTypeButton;
    GtkWidget *getTypesButton;
    GtkWidget *backButton;

    GList *list;

    static ClientInfo client;
    static ReceiverInfo receiver = {.rcvMsgTypes[0 ... BUFF_SIZE-1]= -1};




    list=user_data;
    window=list->data;




    removeWidgets(window);

    gtk_window_resize(GTK_WINDOW(window), 250, 250);

    vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL, PADDING);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    subvbox=gtk_box_new(GTK_ORIENTATION_VERTICAL, PADDING);
    subRegVBox=gtk_box_new(GTK_ORIENTATION_VERTICAL, PADDING);

    for(int i=0; i<5; i++)
    {
        hbox[i]=gtk_box_new(GTK_ORIENTATION_HORIZONTAL, PADDING);
        gtk_box_pack_start(GTK_BOX(vbox), hbox[i], TRUE, FALSE, PADDING);
    }


    IPEntry=gtk_entry_new();
    nameEntry=gtk_entry_new();


    gtk_entry_set_max_length(GTK_ENTRY(IPEntry), INT_DIGITS);
    gtk_entry_set_max_length(GTK_ENTRY(nameEntry), BUFF_SIZE-1);


    msgTypeComboBoxText=gtk_combo_box_text_new();


    list=NULL;
    list=g_list_append(list, window);
    list=g_list_append(list, IPEntry);
    list=g_list_append(list, nameEntry);
    list=g_list_append(list, msgTypeComboBoxText);
    list=g_list_append(list, &client);
    list=g_list_append(list, &receiver);



    IPLabel=gtk_label_new("Server IP: ");
    nameLabel=gtk_label_new("Name: ");


    unregisterAccButton=gtk_button_new_with_label("Unregister");
    registerAccButton=gtk_button_new_with_label("Register");
    optsButton=gtk_button_new_with_label("Options");
    getMsgButton=gtk_button_new_with_label("Get message");
    addTypeButton=gtk_button_new_with_label("+");
    subTypeButton=gtk_button_new_with_label("-");
    getTypesButton=gtk_button_new_with_label("Get\nTypes");
    backButton=gtk_button_new_with_label("Menu");

    gtk_box_pack_start(GTK_BOX(subRegVBox), registerAccButton, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(subRegVBox), unregisterAccButton, TRUE, FALSE, PADDING);

    gtk_box_pack_start(GTK_BOX(subvbox), addTypeButton, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(subvbox), subTypeButton, TRUE, FALSE, PADDING);


    gtk_box_pack_start(GTK_BOX(hbox[0]), IPLabel, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[0]), IPEntry, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[1]), nameLabel, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[1]), nameEntry, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[2]), getTypesButton, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[2]), msgTypeComboBoxText, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[2]), subvbox, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[3]), subRegVBox, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[3]), optsButton, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[3]), getMsgButton, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[4]), backButton, TRUE, FALSE, PADDING);


    g_signal_connect(registerAccButton, "clicked", G_CALLBACK(registerwindow_register), list);
    g_signal_connect(unregisterAccButton, "clicked", G_CALLBACK(registerwindow_unregister), list);
    g_signal_connect(optsButton, "clicked", G_CALLBACK(optionsButton), list);
    g_signal_connect(getMsgButton, "clicked", G_CALLBACK(registerwindow_getMsg), list);
    g_signal_connect(addTypeButton, "clicked", G_CALLBACK(registerwindow_addTypeToRcvArray), list);
    g_signal_connect(subTypeButton, "clicked", G_CALLBACK(registerwindow_subTypeFromRcvArray), list);
    g_signal_connect(getTypesButton, "clicked", G_CALLBACK(registerwindow_getTypes), list);
    g_signal_connect(backButton, "clicked", G_CALLBACK(menuWindow), list);

    gtk_widget_show_all(window);
}

static void exitButton(GtkButton *button,
                       gpointer   user_data)
{
    GList *list;
    list=user_data;

    GtkWidget *window;
    window=list->data;

    gtk_window_close(GTK_WINDOW(window));
}

static void optionsButton(GtkButton *button,
                       gpointer   user_data)
{
    GList *list, *iter;
    GtkWidget *window;
    ClientInfo *client;
    ReceiverInfo *receiver;

    list=user_data;
    iter=list;


    window=iter->data;
    glist_omit(&iter, 4);



    client=iter->data;
    iter=iter->next;

    receiver=iter->data;
    iter=iter->next;



    GtkWidget *vbox;
    GtkWidget *hbox[4];

    GtkWidget *notificationLabel;
    GtkWidget *subscriptionLabel;
    GtkWidget *sendLabel;

    GtkWidget *notificationSwitch;
    GtkWidget *subscriptionSwitch;
    GtkWidget *sendSwitch;

    GtkWidget *backButton;


    removeWidgets(window);


    gtk_window_set_title(GTK_WINDOW(window), "Client");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_widget_set_size_request(window, 200, 100);
    gtk_container_set_border_width (GTK_CONTAINER (window), 20);


    vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL, PADDING);

    for(int i=0; i<4; i++)
    {
        hbox[i]=gtk_box_new(GTK_ORIENTATION_HORIZONTAL, PADDING);
        gtk_box_pack_start(GTK_BOX(vbox), hbox[i], TRUE, FALSE, PADDING);
    }

    notificationLabel=gtk_label_new("No notifications/Notifications");
    subscriptionLabel=gtk_label_new("Temporary subscription/Permanent subscription");
    sendLabel=gtk_label_new("Manual/Automatic");


    notificationSwitch=gtk_switch_new();
    subscriptionSwitch=gtk_switch_new();
    sendSwitch=gtk_switch_new();


    gtk_switch_set_state(GTK_SWITCH(notificationSwitch), receiver->rcvNot == Notifications? True: False);
    gtk_switch_set_state(GTK_SWITCH(subscriptionSwitch), receiver->rcvSub == PermanentSubscription? True: False);
    gtk_switch_set_state(GTK_SWITCH(sendSwitch), receiver->rcvSend == Automatic? True: False);


    backButton=gtk_button_new_with_label("Back");


    g_signal_connect(notificationSwitch, "state-set", G_CALLBACK(options_notificationSwitch), list);
    g_signal_connect(subscriptionSwitch, "state-set", G_CALLBACK(options_subscriptionSwitch), list);
    g_signal_connect(sendSwitch, "state-set", G_CALLBACK(options_sendSwitch), list);
    g_signal_connect(backButton, "clicked", G_CALLBACK(registerButton), list);


    gtk_box_pack_start(GTK_BOX(hbox[0]), notificationLabel, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[0]), notificationSwitch, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[1]), subscriptionLabel, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[1]), subscriptionSwitch, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[2]), sendLabel, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[2]), sendSwitch, TRUE, FALSE, PADDING);
    gtk_box_pack_start(GTK_BOX(hbox[3]), backButton, TRUE, FALSE, PADDING);

    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show_all(window);
}

static gboolean options_notificationSwitch(GtkSwitch *widget,
                                       gboolean   state,
                                       gpointer   user_data)
{
    GList *iter;
    ReceiverInfo *receiver;



    iter=user_data;

    //We omit the unnecessary data
    glist_omit(&iter, 5);

    receiver=iter->data;



    receiver->rcvNot= (state == 0? NoNotifications: Notifications);
    return TRUE;
}

static gboolean options_subscriptionSwitch(GtkSwitch *widget,
                                       gboolean   state,
                                       gpointer   user_data)
{
    GList *iter;
    ReceiverInfo *receiver;



    iter=user_data;

    //We omit the unnecessary data
    glist_omit(&iter, 5);

    receiver=iter->data;



    receiver->rcvSub= (state == 0? TemporarySubscription: PermanentSubscription);
    return TRUE;
}

static gboolean options_sendSwitch(GtkSwitch *widget,
                                   gboolean   state,
                                   gpointer   user_data)
{
    GList *iter;
    ReceiverInfo *receiver;



    iter=user_data;

    //We omit the unnecessary data
    glist_omit(&iter, 5);

    receiver=iter->data;



    receiver->rcvSend= (state == 0? Manual: Automatic);
    return TRUE;
}

static void glist_omit(GList **glist, int number)
{
    while(number>0)
    {
        *glist=(*glist)->next;
        number--;
    }
}
