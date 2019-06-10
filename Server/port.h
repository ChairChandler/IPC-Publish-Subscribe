#ifndef PORTS_H
#define PORTS_H

typedef enum Port
{
    //Server
    Receivers_port=1,
    Senders_port=2,

    //Client
    Message_port=3,
    Error_port=4,
    Notification_port=5,
    Async_port=6,
    Answer_port=7

} Port;

#endif // PORTS_H
