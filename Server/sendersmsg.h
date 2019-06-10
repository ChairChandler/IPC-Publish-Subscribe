#ifndef SENDERSMSG_H
#define SENDERSMSG_H
#include "requests.h"
#include "senders.h"
#include "message.h"

typedef union SndMsg_Data
{
    MsgType msgType;
    MsgType msgTypesArray[BUFF_SIZE];
    Message msg;

} SndMsg_Data;

typedef struct SndMsg
{
    ClientRequest request;
    SenderInfo user;
    SndMsg_Data data;

} SndMsg;

#endif // SENDERSMSG_H
