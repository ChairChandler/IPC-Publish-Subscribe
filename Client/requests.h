#ifndef REQUESTS_H
#define REQUESTS_H

typedef enum ClientRequest
{
    //Receivers
    Register,
    Unregister,
    GetMsg,

    //Senders
    Login,
    Logout,
    NewType,
    RandomType,
    SendMsg,

    //Both
    GetTypes

} ClientRequest;

#endif // REQUESTS_H
