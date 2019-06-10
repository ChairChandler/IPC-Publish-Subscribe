#ifndef CLIENTINFO_H
#define CLIENTINFO_H
#include "msgtype.h"
#include "buffsize.h"

typedef enum RcvInfoNotifications
{
    NoNotifications,
    Notifications //Automatically server sending info to subscribers

} RcvInfoNotifications;

typedef enum RcvInfoSubscription
{
    TemporarySubscription,
    PermanentSubscription

} RcvInfoSubscription;

typedef enum RcvInfoSend
{
    Manual,
    Automatic

} RcvInfoSend;

typedef struct ReceiverInfo
{
    char name[BUFF_SIZE];
    MsgType rcvMsgTypes[BUFF_SIZE];
    RcvInfoNotifications rcvNot;
    RcvInfoSubscription rcvSub;
    RcvInfoSend rcvSend;

} ReceiverInfo;

extern const ReceiverInfo EmptyReceiversInfo;

#endif // RECEIVERS_INFO
