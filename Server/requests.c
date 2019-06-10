#include "requests.h"
#include "port.h"
#include "receiversmsg.h"
#include "sendersmsg.h"
#include "server.h"
#include "msgbuf.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>



static inline char* server_writeError(Error error)
{
    switch(error)
    {
        case MaxLoggedUsers:        return "MaxLoggedUsers";
        case MaxRegisteredUsers:    return "MaxRegisteredUsers";
        case MaxMsgs:               return "MaxMsgs";

        case NoMsg:                 return "NoMsg";
        case NoLoggedUser:          return "NoLoggedUser";
        case NoRegisteredUser:      return "NoRegisteredUser";

        case LoggedUser:            return "LoggedUser";
        case RegisteredUser:        return "RegisteredUser";

        case WrongMsgType:          return "WrongMsgType";
        case NoMsgTypes:            return "NoMsgTypes";
        case MaxMsgTypes:           return "MaxMsgTypes";

        case WrongID:               return "WrongID";
        default:                    return "UnknownError";
    }
}

static void server_notification(ServerInfo *server, MsgType msgType)
{
    for(int i=0; i<BUFF_SIZE; i++)
    {
        if(server->registeredUsers[i].user.rcvNot == Notifications)
        {
            for(int j=0; j<BUFF_SIZE; j++)
            {
                if(server->registeredUsers[i].user.rcvMsgTypes[j] == msgType)
                {
                    Wrapper toReceiver;

                    toReceiver.mtype=Notification_port;
                    toReceiver.data.isNotification=True;

                    int fileDesc;
                    fileDesc=msgget(server_generateInt(server->registeredUsers[i].user.name), 0666);

                    msgsnd(fileDesc, &toReceiver, sizeof(Wrapper)-sizeof(MsgType), IPC_NOWAIT);
                    printf("[Server] Sended notification to %s\n", server->registeredUsers[i].user.name);
                    break;
                }
            }
        }
    }
}

static int server_error(char name[BUFF_SIZE] , Error error)
{
    Wrapper msg;

    msg.mtype=Error_port;
    msg.data.error=error;

    int fileDesc=msgget(server_generateInt(name), 0666);
    printf("[%s] Error: %s\n", name, server_writeError(error));

    return msgsnd(fileDesc, &msg, sizeof(Wrapper) - sizeof(MsgType), IPC_NOWAIT);
}

int doRequest_Login(ServerInfo *server, SndMsg *sndMsg)
{
    if(server_findRegisteredUser(server, sndMsg->user.name) == -1)
    {
        server_error(sndMsg->user.name, NoRegisteredUser);
        return -1;
    }
    if(server_findLoggedUser(server, &sndMsg->user) != -1)
    {
        server_error(sndMsg->user.name, LoggedUser);
        return -1;
    }
    else
    {
        if(server_addLoggedUser(server, &sndMsg->user) == -1)
        {
            server_error(sndMsg->user.name, MaxLoggedUsers);
            return -1;
        }
        else return server_sendActivity(sndMsg, NULL);
    }
}

int doRequest_Logout(ServerInfo *server, SndMsg *sndMsg)
{
    if(server_isCorrectLoggedUser(server, &sndMsg->user) == 0)
    {
        server_error(sndMsg->user.name, WrongID);
        return -1;
    }
    if(server_removeLoggedUser(server, &sndMsg->user) == -1)
    {
        server_error(sndMsg->user.name, NoLoggedUser);
        return -1;
    }
    else return server_sendActivity(sndMsg, NULL);
}

int doRequest_Register(ServerInfo *server, RcvMsg *rcvMsg)
{
    if(server_findRegisteredUser(server, rcvMsg->data.user.name) != -1)
    {
        server_error(rcvMsg->data.user.name, RegisteredUser);
        return -1;
    }
    else
    {
        if(server_addRegisteredUser(server, &rcvMsg->data.user) == -1)
        {
            server_error(rcvMsg->data.user.name, MaxRegisteredUsers);
            return -1;
        }
        else return server_sendActivity(NULL, rcvMsg);
    }
}

int doRequest_Unregister(ServerInfo *server, RcvMsg *rcvMsg)
{
    if(server_removeRegisteredUser(server, &rcvMsg->data.user) == -1)
    {
        server_error(rcvMsg->data.user.name, NoRegisteredUser);
        return -1;
    }
    else return server_sendActivity(NULL, rcvMsg);
}

int doRequest_NewType(ServerInfo *server, SndMsg *sndMsg)
{
    if(server_findLoggedUser(server, &sndMsg->user) == -1)
    {
        server_error(sndMsg->user.name, NoLoggedUser);
        return -1;
    }
    else if(server_findClientType(server, sndMsg->data.msgType) == -1)
    {
        if(server_addClientType(server, sndMsg->data.msgType) == -1)
        {
            server_error(sndMsg->user.name, MaxMsgTypes);
            return -1;
        }
        else return server_sendActivity(sndMsg, NULL);
    }
    else
    {
        server_error(sndMsg->user.name, WrongMsgType);
        return -1;
    }
}

int doRequest_RandomType(ServerInfo *server, SndMsg *sndMsg)
{
    if(server_findLoggedUser(server, &sndMsg->user) == -1)
    {
        server_error(sndMsg->user.name, NoLoggedUser);
        return -1;
    }
    else
    {
        for(MsgType i=1; i<=BUFF_SIZE; i++)
        {
            if(server_findClientType(server, i) == -1)
            {
                if(server_addClientType(server, i) == -1)
                {
                    server_error(sndMsg->user.name, MaxMsgTypes);
                    return -1;
                }
                else
                {
                    Wrapper toSender;
                    int fileDesc;

                    fileDesc=msgget(server_generateInt(sndMsg->user.name), 0666);
                    toSender.mtype=Message_port;
                    toSender.data.sndMsg.data.msgType=i;

                    msgsnd(fileDesc, &toSender, sizeof(Wrapper)-sizeof(MsgType), IPC_NOWAIT);
                    return 0;
                }
            }
        }

        return -1;
    }
}

int doRequest_SendMsg(ServerInfo *server, SndMsg *sndMsg)
{
    if(server_findLoggedUser(server, &sndMsg->user) == -1)
    {
        server_error(sndMsg->user.name, NoLoggedUser);
        return -1;
    }
    else
    {
        if(server_isEmptyMessageInUsersMsgBox(server) == -1)
        {
            server_error(sndMsg->user.name, MaxMsgs);
            return -1;
        }
        else
        {
            printf("[%s] Msg received: %s\n", sndMsg->user.name, sndMsg->data.msg.text);
            server_addMessage(server, sndMsg->data.msg);
            server_notification(server, sndMsg->data.msg.msgType);
            return server_sendActivity(sndMsg, NULL);
        }
    }

}

int doRequest_GetMsg(ServerInfo *server, RcvMsg *rcvMsg)
{
    int usrPos;
    if((usrPos=server_findRegisteredUser(server, rcvMsg->data.user.name)) == -1)
    {
        server_error(rcvMsg->data.user.name, NoRegisteredUser);
        return -1;
    }
    else
    {
        int msgPos;

        msgPos=server_findMsgToSndPos(&server->registeredUsers[usrPos]);

        if(msgPos == -1)
        {
            server_error(rcvMsg->data.user.name, NoMsg);
            return -1;
        }
        else
        {
            Message *msg;
            msg=server_getMsgFromUserMsgBox(&server->registeredUsers[usrPos], msgPos);

            if(msg != NULL)
            {
                Wrapper toReceiver;
                toReceiver.mtype=Message_port;
                toReceiver.data.rcvMsg.data.msg=*msg;


                int fileDesc=msgget(server_generateInt(rcvMsg->data.user.name), 0666);
                msgsnd(fileDesc, &toReceiver, sizeof(Wrapper)-sizeof(MsgType), IPC_NOWAIT);

                printf("[%s] Sended: %s\n", rcvMsg->data.user.name, toReceiver.data.rcvMsg.data.msg.text);

                free(msg);
                return 0;
            }
            else
            {
                printf("[%s] Something wrong with msg\n", rcvMsg->data.user.name);
                return -1;
            }
        }
    }

}

int doRequest_GetTypes_sender(ServerInfo *server, SndMsg *sndMsg)
{
    if(!server_isEmptyClientTypes(server))
    {
        server_error(sndMsg->user.name, NoMsgTypes);
        return -1;
    }
    else
    {
        Wrapper toSender;
        toSender.mtype=Message_port;
        memcpy(toSender.data.sndMsg.data.msgTypesArray, server->msgTypes, sizeof(MsgType) * BUFF_SIZE);

        int fileDesc;
        fileDesc=msgget(server_generateInt(sndMsg->user.name), 0666);

        msgsnd(fileDesc, &toSender, sizeof(Wrapper)-sizeof(MsgType), IPC_NOWAIT);

        printf("[%s] Types sended\n", sndMsg->user.name);
        return 0;
    }
}

int doRequest_GetTypes_receiver(ServerInfo *server, RcvMsg *rcvMsg)
{
    if(!server_isEmptyClientTypes(server))
    {
        server_error(rcvMsg->data.user.name, NoMsgTypes);
        return -1;
    }
    else
    {
        Wrapper toSender;
        toSender.mtype=Message_port;
        memcpy(toSender.data.rcvMsg.data.msgTypeArray, server->msgTypes, sizeof(MsgType) * BUFF_SIZE);

        int fileDesc;
        fileDesc=msgget(server_generateInt(rcvMsg->data.user.name), 0666);

        msgsnd(fileDesc, &toSender, sizeof(Wrapper)-sizeof(MsgType), IPC_NOWAIT);

        printf("[%s] Types sended\n", rcvMsg->data.user.name);
        return 0;
    }
}
