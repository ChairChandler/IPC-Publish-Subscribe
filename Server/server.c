#include "server.h"
#include <string.h>
#include "bool.h"
#include "port.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>


static int server_findFreeSlotRegistered(ServerInfo *server)
{
    for(int i=0; i<BUFF_SIZE; i++)
    {
        if(!strcmp(server->registeredUsers[i].user.name, EmptyReceiversInfo.name))
            return i;
    }

    return -1;
}

static int server_findFreeSlotLogged(ServerInfo *server)
{
    for(int i=0; i<BUFF_SIZE; i++)
    {
        if(server->loggedUsers[i].ID == EmptySendersInfo.ID &&
                !strcmp(server->loggedUsers[i].name, EmptySendersInfo.name))

            return i;
    }

    return -1;
}

static int server_isClientInterested(ReceiverInfo user, MsgType msgType)
{
    for(int i=0; i<BUFF_SIZE; i++)
    {
        if(user.rcvMsgTypes[i] == msgType)
        {
            return 0;
        }
    }

    return -1;
}

int server_findClientType(ServerInfo *server, MsgType msgType)
{
    for(int i=0; i<BUFF_SIZE; i++)
    {
        if(server->msgTypes[i] == msgType)
            return i;
    }

    return -1;
}

int server_findRegisteredUser(ServerInfo *server, char name[BUFF_SIZE])
{
    for(int i=0; i<BUFF_SIZE; i++)
    {
        if(!strcmp(server->registeredUsers[i].user.name, name))
            return i;
    }

    return -1;
}

int server_findLoggedUser(ServerInfo *server, SenderInfo *user)
{
    for(int i=0; i<BUFF_SIZE; i++)
    {
        if(strcmp(server->loggedUsers[i].name, user->name) == 0)
            return i;
    }

    return -1;
}

int server_addRegisteredUser(ServerInfo *server, ReceiverInfo *user)
{
    int freeSlot;
    freeSlot=server_findFreeSlotRegistered(server);

    if(freeSlot == -1)
        return -1;
    else
    {
        server->registeredUsers[freeSlot].user=*user;
        server->registeredUsers[freeSlot].registerTime=time(NULL);

        for(int i=0; i<BUFF_SIZE; i++) //msgBox
        {
            server->registeredUsers[freeSlot].msgBox[i]=EmptyMessage;
        }

        return 0;
    }
}

int server_addLoggedUser(ServerInfo *server, SenderInfo *user)
{
    int freeSlot;
    freeSlot=server_findFreeSlotLogged(server);

    if(freeSlot == -1)
        return -1;
    else
    {
        server->loggedUsers[freeSlot]=*user;
        return 0;
    }
}

int server_removeRegisteredUser(ServerInfo *server, ReceiverInfo *user)
{
    for(int i=0; i<BUFF_SIZE; i++) //user
    {
        if(!strcmp(server->registeredUsers[i].user.name, user->name))
        {
            server->registeredUsers[i].user=EmptyReceiversInfo;
            server->registeredUsers[i].registerTime= -1;

            for(int j=0; j<BUFF_SIZE; j++) //msgBox
            {
                server->registeredUsers[i].msgBox[j]=EmptyMessage;
            }

            return 0;
        }
    }

    return -1;
}

int server_removeLoggedUser(ServerInfo *server, SenderInfo *user)
{
    for(int i=0; i<BUFF_SIZE; i++)
    {
        if(!strcmp(server->loggedUsers[i].name, user->name) &&
                server->loggedUsers[i].ID == user->ID)
        {
            server->loggedUsers[i]=EmptySendersInfo;
            return 0;
        }
    }

    return -1;
}

int server_isCorrectLoggedUser(ServerInfo *server, SenderInfo *user)
{
    for(int i=0; i<BUFF_SIZE; i++)
    {
        if(strcmp(server->loggedUsers[i].name, user->name) == 0 &&
                server->loggedUsers[i].ID != user->ID)
        {
            return 0;
        }
    }

    return -1;
}

int server_addClientType(ServerInfo *server, MsgType msgType)
{
    for(int i=0; i<BUFF_SIZE; i++)
    {
        if(server->msgTypes[i] == EmptyMsgType)
        {
            server->msgTypes[i]=msgType;
            return 0;
        }
    }

    return -1;
}

void server_addMessage(ServerInfo *server, Message msg)
{
    for(int i=0; i<BUFF_SIZE; i++) // user
    {
        if(server_isClientInterested(server->registeredUsers[i].user, msg.msgType) == 0)
        {
            int pos=server_findEmptyMsg(server->registeredUsers[i]);

            if(pos != -1)
            {
                server->registeredUsers[i].msgBox[pos]=msg;
            }
            else
                continue;
        }
    }
}

int server_isEmptyClientTypes(ServerInfo *server)
{
    for(int i=0; i<BUFF_SIZE; i++)
    {
        if(server->msgTypes[i] != EmptyMsgType)
        {
            return -1;
        }
    }

    return 0;
}

int server_isEmptyMessageInUsersMsgBox(ServerInfo *server)
{
    int counter;
    counter=0;

    for(int i=0; i<BUFF_SIZE; i++) //user
    {
        for(int j=0; j<BUFF_SIZE; j++) // msgType
        {
            if(server->registeredUsers[i].msgBox[j].msgType == EmptyMessage.msgType)
            {
                counter++;
                break;
            }
        }
    }

    return counter != 0? 0 : -1;
}

int server_findMsgToSndPos(RegisterUserInfo *user)
{
    int pos;
    size_t priority;
    Bool isFirstTime;

    pos= -1;
    priority=0;
    isFirstTime=True;

    for(int i=0; i<BUFF_SIZE; i++) //msgBox
    {
        if(user->msgBox[i].msgType != EmptyMsgType)
        {
            if(isFirstTime==True)
            {
                pos=i;
                priority=user->msgBox[i].priority;
                isFirstTime=False;
            }
            else if(user->msgBox[i].priority > priority)
            {
                pos=i;
                priority=user->msgBox[i].priority;
            }
        }
    }

    return pos;
}

int server_findEmptyMsg(RegisterUserInfo user)
{
    for(int i=0; i<BUFF_SIZE; i++)
    {
        if(strcmp(user.msgBox[i].text, EmptyMessage.text) == 0)
        {
            return i;
        }
    }

    return -1;
}

int server_generateInt(char name[BUFF_SIZE])
{
    int sum;
    sum=0;

    for(int i=0; i<(int)strlen(name); i++)
    {
        sum+=(int)name[i];
    }

    return sum;
}

int server_sendActivity(SndMsg *sndMsg, RcvMsg *rcvMsg)
{
    Wrapper wrapper;

    wrapper.mtype=Answer_port;
    wrapper.data.emptyData=(char)0;

    if(sndMsg != NULL)
    {
        int fd=msgget(server_generateInt(sndMsg->user.name), 0666);
        return msgsnd(fd, &wrapper, sizeof(Wrapper)-sizeof(MsgType), IPC_NOWAIT);
    }

    if(rcvMsg != NULL)
    {
        int fd=msgget(server_generateInt(rcvMsg->data.user.name), 0666);
        return msgsnd(fd, &wrapper, sizeof(Wrapper)-sizeof(MsgType), IPC_NOWAIT);
    }

    return 0;
}

Message* server_getMsgFromUserMsgBox(RegisterUserInfo *user, int msgPos)
{
    if(strcmp(user->msgBox[msgPos].text, EmptyMessage.text) == 0)
    {
        return NULL;
    }
    else
    {
        Message *msg;
        msg=(Message*)malloc(sizeof(Message));

        *msg=user->msgBox[msgPos];
        user->msgBox[msgPos]=EmptyMessage;

        return msg;
    }
}

int server_putMsgIntoUserMsgBox(RegisterUserInfo *user, Message msg)
{
    for(int i=0; i<BUFF_SIZE; i++)
    {
        if(strcmp(user->msgBox[i].text, EmptyMessage.text) == 0)
        {
            user->msgBox[i]=msg;
            return 0;
        }
    }

    return -1;
}
