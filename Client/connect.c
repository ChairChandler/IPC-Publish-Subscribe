#include "connect.h"
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include "receiversmsg.h"
#include "sendersmsg.h"
#include "port.h"
#include "error.h"
#include "msgbuf.h"

#define WAIT_TIME_MS 1000

static int client_generateInt(const char name[BUFF_SIZE])
{
    int sum;
    sum=0;

    for(int i=0; i<(int)strlen(name); i++)
    {
        sum+=(int)name[i];
    }

    return sum;
}

static int client_checkError(ClientInfo *client)
{
    int status;
    Wrapper errMsg;
    usleep(WAIT_TIME_MS);

    status=(int)msgrcv(client->clientFileDesc, &errMsg, sizeof(Wrapper)-sizeof(MsgType), Error_port, IPC_NOWAIT);

    if(status != -1)
    {
        free(client->lastError);
        client->lastError=(Error*)malloc(sizeof(Error));
        *client->lastError=errMsg.data.error;
    }

    return status == -1? 0: -1;
}

int client_isError(ClientInfo *client)
{
    if(client->lastError==NULL)
        return -1;
    else
    {
        free(client->lastError);
        client->lastError=NULL;
        return 0;
    }
}

Error* client_getError(ClientInfo *client)
{
    if(client->lastError==NULL)
        return NULL;
    else
    {
        Error *err;
        err=(Error*)malloc(sizeof(Error));
        *err = *client->lastError;
        client_clearError(client);

        return err;
    }
}

void client_clearError(ClientInfo *client)
{
    free(client->lastError);
    client->lastError=NULL;
}

int client_isServerActivity(ClientInfo *client)
{
    Wrapper wrapper;

    usleep(WAIT_TIME_MS);

    int status=msgrcv(client->clientFileDesc, &wrapper, sizeof(Wrapper)-sizeof(MsgType), Answer_port, IPC_NOWAIT);

    return status;
}

int client_init(ClientInfo *client, const char name[BUFF_SIZE], int IP)
{
    client->lastError=NULL;
    client->serverFileDesc=msgget(IP, 0666);
    client->clientFileDesc=msgget(client_generateInt(name), 0666|IPC_CREAT);
    return client->serverFileDesc == -1 || client->clientFileDesc == -1 ? -1 : 0;
}

int client_register(ClientInfo *client, ReceiverInfo *receiver)
{
    Wrapper msg;

    msg.mtype=Receivers_port;
    msg.data.rcvMsg.request=Register;
    msg.data.rcvMsg.data.user=*receiver;

    if(msgsnd(client->serverFileDesc, &msg, sizeof(Wrapper)-sizeof(MsgType), IPC_NOWAIT) == -1)
        return -1;
    else
        return client_checkError(client);
}

int client_unregister(ClientInfo *client, ReceiverInfo *receiver)
{
    Wrapper msg;

    msg.mtype=Receivers_port;
    msg.data.rcvMsg.request=Unregister;
    msg.data.rcvMsg.data.user=*receiver;

    if(msgsnd(client->serverFileDesc, &msg, sizeof(Wrapper)-sizeof(MsgType), IPC_NOWAIT) == -1)
        return -1;
    else
        return client_checkError(client);
}

Message* client_getMsg(ClientInfo *client, ReceiverInfo *receiver)
{
    Wrapper msg;

    msg.mtype=Receivers_port;
    msg.data.rcvMsg.request=GetMsg;
    msg.data.rcvMsg.data.user=*receiver;

    if(msgsnd(client->serverFileDesc, &msg, sizeof(Wrapper)-sizeof(MsgType), IPC_NOWAIT) == -1)
        return NULL;
    else
    {
        if(client_checkError(client) == -1 || client_isError(client) == 0)
            return NULL;
        else
        {
            int status=(int)msgrcv(client->clientFileDesc, &msg, sizeof(Wrapper)-sizeof(MsgType), Message_port, NULL);

            if(status == -1)
                return NULL;
            else
            {
                Message *message;
                message=(Message*)malloc(sizeof(Message));
                *message=msg.data.rcvMsg.data.msg;

                return message;
            }
        }
    }
}

int client_login(ClientInfo *client, SenderInfo *sender)
{
    Wrapper msg;

    msg.mtype=Senders_port;
    msg.data.sndMsg.request=Login;
    msg.data.sndMsg.user=*sender;

    if(msgsnd(client->serverFileDesc, &msg, sizeof(Wrapper)-sizeof(MsgType), IPC_NOWAIT) == -1)
        return -1;
    else
        return client_checkError(client);
}

int client_logout(ClientInfo *client, SenderInfo *sender)
{
    Wrapper msg;

    msg.mtype=Senders_port;
    msg.data.sndMsg.request=Logout;
    msg.data.sndMsg.user=*sender;

    if(msgsnd(client->serverFileDesc, &msg, sizeof(Wrapper)-sizeof(MsgType), IPC_NOWAIT) == -1)
        return -1;
    else
        return client_checkError(client);
}

int client_newType(ClientInfo *client, SenderInfo *sender, MsgType *msgType)
{
    Wrapper msg;

    msg.mtype=Senders_port;
    msg.data.sndMsg.request=NewType;
    msg.data.sndMsg.user=*sender;
    msg.data.sndMsg.data.msgType=*msgType;

    if(msgsnd(client->serverFileDesc, &msg, sizeof(Wrapper)-sizeof(MsgType), IPC_NOWAIT) == -1)
        return -1;
    else
        return client_checkError(client);
}

MsgType* client_randomType(ClientInfo *client, SenderInfo *sender)
{
    Wrapper msg;

    msg.mtype=Senders_port;
    msg.data.sndMsg.request=RandomType;
    msg.data.sndMsg.user=*sender;

    if(msgsnd(client->serverFileDesc, &msg, sizeof(Wrapper)-sizeof(MsgType), IPC_NOWAIT) == -1)
        return NULL;
    else
    {
        if(client_checkError(client) == -1 || client_isError(client) == 0)
            return NULL;
        else
        {
            int status=(int)msgrcv(client->clientFileDesc, &msg, sizeof(Wrapper)-sizeof(MsgType), Message_port, NULL);

            if(status == -1)
                return NULL;
            else
            {
                MsgType *msgType;
                msgType=(MsgType*)malloc(sizeof(MsgType));
                *msgType=msg.data.sndMsg.data.msgType;

                return msgType;
            }
        }
    }
}

int client_sendMsg(ClientInfo *client, SenderInfo *sender, Message *message)
{
    Wrapper msg;

    msg.mtype=Senders_port;
    msg.data.sndMsg.request=SendMsg;
    msg.data.sndMsg.user=*sender;
    msg.data.sndMsg.data.msg.msgType=message->msgType;
    msg.data.sndMsg.data.msg.priority=message->priority;
    strcpy(msg.data.sndMsg.data.msg.text, message->text);

    if(msgsnd(client->serverFileDesc, &msg, sizeof(Wrapper)-sizeof(MsgType), IPC_NOWAIT) == -1)
        return -1;
    else
        return client_checkError(client);
}

MsgType* client_getTypes(ClientInfo *client, ReceiverInfo *receiver, SenderInfo *sender)
{
    Wrapper msg;

    if(sender != NULL)
    {
        msg.mtype=Senders_port;
        msg.data.sndMsg.request=GetTypes;
        msg.data.sndMsg.user=*sender;
    }
    else if( receiver != NULL)
    {
        msg.mtype=Receivers_port;
        msg.data.rcvMsg.request=GetTypes;
        msg.data.rcvMsg.data.user=*receiver;
    }
    else return NULL;



    if(msgsnd(client->serverFileDesc, &msg, sizeof(Wrapper)-sizeof(MsgType), IPC_NOWAIT) == -1)
        return NULL;
    else
    {
        if(client_checkError(client) == -1 || client_isError(client) == 0)
            return NULL;
        else
        {
            int status=(int)msgrcv(client->clientFileDesc, &msg, sizeof(Wrapper)-sizeof(MsgType), Message_port, NULL);

            if(status == -1)
                return NULL;
            else
            {
                MsgType *msgType;
                msgType=(MsgType*)malloc(sizeof(MsgType) * BUFF_SIZE);

                for(int i=0; i<BUFF_SIZE; i++)
                {
                    if(sender != NULL)
                    {
                        msgType[i]=msg.data.sndMsg.data.msgTypesArray[i];
                    }
                    else
                    {
                        msgType[i]=msg.data.rcvMsg.data.msgTypeArray[i];
                    }
                }

                return msgType;
            }
        }
    }
}

int client_isNotification(ClientInfo *client)
{
    Wrapper msg;

    if(msgrcv(client->clientFileDesc, &msg, sizeof(Wrapper)-sizeof(MsgType), Notification_port, IPC_NOWAIT) != -1)
    {
        return 0;
    }
    else return -1;
}

Message* client_getAsyncMsg(ClientInfo *client)
{
    Wrapper msg;

    if(msgrcv(client->clientFileDesc, &msg, sizeof(Wrapper)-sizeof(MsgType), Async_port, IPC_NOWAIT) != -1)
    {
        Message *message;
        message=(Message*)malloc(sizeof(Message));
        *message=msg.data.rcvMsg.data.msg;

        return message;
    }
    else return NULL;
}

void client_exit(ClientInfo *client)
{
     msgctl(client->clientFileDesc, IPC_RMID, NULL);
}
