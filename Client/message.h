#ifndef MESSAGE_H
#define MESSAGE_H
#include <stdlib.h>
#include "msgtype.h"
#include "buffsize.h"

typedef struct Message
{
    char text[BUFF_SIZE];
    size_t priority;
    MsgType msgType;

} Message;

extern const Message EmptyMessage;

#endif // MESSAGE_H
