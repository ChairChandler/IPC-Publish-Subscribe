#ifndef RECEIVERSMSG_H
#define RECEIVERSMSG_H
#include "message.h"
#include "receivers.h"
#include "requests.h"

typedef union RcvMsg_Data
{
    ReceiverInfo user;
    Message msg;
    MsgType msgTypeArray[BUFF_SIZE];

} RcvMsg_Data;

typedef struct RcvMsg
{
    ClientRequest request;
    RcvMsg_Data data;

} RcvMsg;

#endif // RECEIVERSMSG_H
