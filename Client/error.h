#ifndef ERROR_H
#define ERROR_H

typedef enum Error
{
    MaxLoggedUsers,
    MaxRegisteredUsers,
    MaxMsgs,

    NoMsg,
    NoLoggedUser,
    NoRegisteredUser,

    LoggedUser,
    RegisteredUser,

    WrongMsgType,
    NoMsgTypes,
    MaxMsgTypes,

    WrongID

} Error;

#endif // ERROR_H
