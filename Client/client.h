#ifndef CLIENT_H
#define CLIENT_H
#include "bool.h"
#include "message.h"
#include "error.h"

typedef struct ClientInfo
{
    int clientFileDesc;
    int serverFileDesc;
    Error *lastError;

} ClientInfo;

#endif // CLIENT_H
