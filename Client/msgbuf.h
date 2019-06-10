#ifndef MSGBUF_H
#define MSGBUF_H
#include "msgtype.h"
#include "error.h"
#include "receiversmsg.h"
#include "sendersmsg.h"
#include "bool.h"

typedef union Wrapper_data
{
    Error error;
    RcvMsg rcvMsg;
    SndMsg sndMsg;
    Bool isNotification;
    char emptyData;

} Wrapper_data;

typedef struct msgbuf
{
    MsgType mtype;
    Wrapper_data data;

} Wrapper;

#endif // MSGBUF_H
