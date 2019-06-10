#ifndef SERVER_H
#define SERVER_H

#include "receivers.h"
#include "senders.h"
#include "message.h"
#include "msgbuf.h"

typedef struct RegisterUserInfo
{
    ReceiverInfo  user;
    time_t registerTime;
    Message msgBox[BUFF_SIZE];

} RegisterUserInfo;

typedef struct ServerInfo
{
    int fileDesc;

    RegisterUserInfo registeredUsers[BUFF_SIZE];
    SenderInfo loggedUsers[BUFF_SIZE];

    MsgType msgTypes[BUFF_SIZE];

} ServerInfo;

int server_findClientType(ServerInfo*, MsgType);
int server_findRegisteredUser(ServerInfo*, char name[BUFF_SIZE]);
int server_findLoggedUser(ServerInfo*, SenderInfo*);

int server_addRegisteredUser(ServerInfo*, ReceiverInfo*);
int server_addLoggedUser(ServerInfo*, SenderInfo*);

int server_removeRegisteredUser(ServerInfo*, ReceiverInfo*);
int server_removeLoggedUser(ServerInfo*, SenderInfo*);

int server_addClientType(ServerInfo *server, MsgType msgType);
void server_addMessage(ServerInfo *server, Message msg);

int server_isEmptyClientTypes(ServerInfo *server);
int server_isEmptyMessageInUsersMsgBox(ServerInfo *server);

int server_findEmptyMsg(RegisterUserInfo);
int server_findMsgToSndPos(RegisterUserInfo*);
int server_generateInt(char name[BUFF_SIZE]);

int server_sendActivity(SndMsg *sndMsg, RcvMsg *rcvMsg);
int server_isCorrectLoggedUser(ServerInfo *server, SenderInfo *user);

int server_putMsgIntoUserMsgBox(RegisterUserInfo *user, Message msg);
Message* server_getMsgFromUserMsgBox(RegisterUserInfo *user, int msgPos);

#endif // SERVER_H
