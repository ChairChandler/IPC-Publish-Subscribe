#include "connect.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "server.h"
#include "bool.h"
#include "requests.h"
#include "receiversmsg.h"
#include "sendersmsg.h"
#include "port.h"
#include "msgbuf.h"

//After 300s (5min) user with temporary subscription
//will be unregistered from subscription system
#define END_SUB_TIME_SECONDS 300

static int init(ServerInfo *server, int IP)
{
    srand(time(NULL));

    for(int i=0; i<BUFF_SIZE; i++)
    {
        server->registeredUsers[i].user=EmptyReceiversInfo;
        server->registeredUsers[i].registerTime= -1;

        for(int j=0; j<BUFF_SIZE; j++)
        {
            server->registeredUsers[i].msgBox[j]=EmptyMessage;
        }

        server->loggedUsers[i]=EmptySendersInfo;


        server->msgTypes[i]=EmptyMsgType;
    }

    return server->fileDesc=msgget(IP, 0666|IPC_CREAT);
}

static RcvMsg* getRcvMsg(ServerInfo *server)
{
    Wrapper wrapper;
    RcvMsg *rcvMsg;

    rcvMsg=(RcvMsg*)malloc(sizeof(RcvMsg));

    if(msgrcv(server->fileDesc, &wrapper, sizeof(Wrapper)-sizeof(MsgType), Receivers_port, IPC_NOWAIT) == -1)
    {
        free(rcvMsg);
        rcvMsg=NULL;
    }
    else
    {
        *rcvMsg=wrapper.data.rcvMsg;
    }

    return rcvMsg;
}

static SndMsg* getSndMsg(ServerInfo *server)
{
    Wrapper wrapper;
    SndMsg *sndMsg;

    sndMsg=(SndMsg*)malloc(sizeof(SndMsg));

    if(msgrcv(server->fileDesc, &wrapper, sizeof(Wrapper)-sizeof(MsgType), Senders_port, IPC_NOWAIT) == -1)
    {
        free(sndMsg);
        sndMsg=NULL;
    }
    else
    {
        *sndMsg=wrapper.data.sndMsg;
    }

    return sndMsg;
}

static void doRcvRequest(ServerInfo *server, RcvMsg *rcvMsg)
{
    switch(rcvMsg->request)
    {
        case Register:
        {
            printf("[%s] Register\n", rcvMsg->data.user.name);
            doRequest_Register(server, rcvMsg);
        }
            break;

        case Unregister:
        {
            printf("[%s] Unregister\n", rcvMsg->data.user.name);
            doRequest_Unregister(server, rcvMsg);
        }
            break;

        case GetMsg:
        {
            printf("[%s] GetMsg\n", rcvMsg->data.user.name);
            doRequest_GetMsg(server, rcvMsg);
        }
            break;

        case GetTypes:
        {
            printf("[%s] GetTypes\n", rcvMsg->data.user.name);
            doRequest_GetTypes_receiver(server, rcvMsg);
        }
            break;

        default:
            printf("[%s] Unrecognised request\n", rcvMsg->data.user.name);
            return;
    }
}

static void doSndRequest(ServerInfo *server, SndMsg *sndMsg)
{
    switch(sndMsg->request)
    {
        case Login:
        {
            printf("[%s] Login\n", sndMsg->user.name);
            doRequest_Login(server, sndMsg);
        }
            break;

        case Logout:
        {
            printf("[%s] Logout\n", sndMsg->user.name);
            doRequest_Logout(server, sndMsg);
        }
            break;

        case NewType:
        {
            printf("[%s] NewType\n", sndMsg->user.name);
            doRequest_NewType(server, sndMsg);
        }
            break;

        case RandomType:
        {
            printf("[%s] Randomtype\n", sndMsg->user.name);
            doRequest_RandomType(server, sndMsg);
        }
            break;

        case SendMsg:
        {
            printf("[%s] SendMsg\n", sndMsg->user.name);
            doRequest_SendMsg(server, sndMsg);
        }
            break;

        case GetTypes:
        {
            printf("[%s] GetTypes\n", sndMsg->user.name);
            doRequest_GetTypes_sender(server, sndMsg);
        }
            break;

        default:
            printf("[%s] Unrecognised request\n", sndMsg->user.name);
            return;
    }
}

static void sndAsyncMsgToClients(ServerInfo *server)
{
    for(int i=0; i<BUFF_SIZE; i++) // user
    {
        if(server->registeredUsers[i].user.rcvSend == Automatic)
        {
            int pos;
            if((pos=server_findMsgToSndPos(&server->registeredUsers[i])) == -1)
                continue;
            else
            {
                Message *msg;
                msg=server_getMsgFromUserMsgBox(&server->registeredUsers[i], pos);

                if(msg != NULL)
                {
                    Wrapper toReceiver;

                    toReceiver.mtype=Async_port;
                    toReceiver.data.rcvMsg.data.msg=*msg;

                    int fileDesc;
                    fileDesc=msgget(server_generateInt(server->registeredUsers[i].user.name), 0666);

                    msgsnd(fileDesc, &toReceiver, sizeof(Wrapper)-sizeof(MsgType), IPC_NOWAIT);
                    printf("[Server] Sended async msg to: %s\n", server->registeredUsers[i].user.name);

                    free(msg);
                }
                else
                {
                     printf("[Server] Error while sending async msg to: %s\n", server->registeredUsers[i].user.name);
                }
            }
        }
    }
}

static void checkRegisterTime(ServerInfo *server)
{
    for(int i=0; i<BUFF_SIZE; i++)
    {
        if(strcmp(server->registeredUsers[i].user.name, EmptyReceiversInfo.name) != 0 &&
                server->registeredUsers[i].user.rcvSub == TemporarySubscription &&
                time(NULL)-server->registeredUsers[i].registerTime > END_SUB_TIME_SECONDS)
        {
            printf("[Time] User sub-time ended: %s\n", server->registeredUsers[i].user.name);
            server->registeredUsers[i].user=EmptyReceiversInfo;
            server->registeredUsers[i].registerTime= -1;
        }
    }
}

int run(int IP)
{
    ServerInfo *server;
    RcvMsg *rcvMsg;
    SndMsg *sndMsg;

    server=(ServerInfo*)malloc(sizeof(ServerInfo));
    printf("[Server] Server started\n");

    if(init(server, IP) == -1)
    {
        printf("[Server] Init failure!\n");
        msgctl(server->fileDesc, IPC_RMID, NULL);
        free(server);
        return -1;
    }
    else
    {
        printf("[Server] Init successfull\n");

        while(True)
        {
            rcvMsg=getRcvMsg(server);
            sndMsg=getSndMsg(server);

            if(rcvMsg != NULL)
            {
                printf("[Request] Receiver: %s\n", rcvMsg->data.user.name);
                doRcvRequest(server, rcvMsg);
                free(rcvMsg);
            }

            if(sndMsg != NULL)
            {
                printf("[Request] Sender: %s\n", sndMsg->user.name);
                doSndRequest(server, sndMsg);
                free(sndMsg);
            }

            sndAsyncMsgToClients(server);
            checkRegisterTime(server);
        }
    }
}
