#ifndef CONNECT_H
#define CONNECT_H
#include "message.h"
#include "msgtype.h"
#include "receivers.h"
#include "senders.h"
#include "client.h"
#include "msgbuf.h"

int client_isServerActivity(ClientInfo *client);
Error* client_getError(ClientInfo *client);
void client_clearError(ClientInfo *client);

int client_init(ClientInfo *client, const char name[BUFF_SIZE], int IP);
void client_exit(ClientInfo *client);

int client_register(ClientInfo *client, ReceiverInfo *receiver);
int client_unregister(ClientInfo *client, ReceiverInfo *receiver);
Message* client_getMsg(ClientInfo *client, ReceiverInfo *receiver);

int client_login(ClientInfo *client, SenderInfo *sender);
int client_logout(ClientInfo *client, SenderInfo *sender);
int client_newType(ClientInfo *client, SenderInfo *sender, MsgType *msgType);
MsgType* client_randomType(ClientInfo *client, SenderInfo *sender);
int client_sendMsg(ClientInfo *client, SenderInfo *sender, Message *msg);
MsgType* client_getTypes(ClientInfo *client, ReceiverInfo *receiver, SenderInfo *sender);

int client_isNotification(ClientInfo *client);
int client_isError(ClientInfo *client);

Message* client_getAsyncMsg(ClientInfo *client);

#endif // CONNECT_H
