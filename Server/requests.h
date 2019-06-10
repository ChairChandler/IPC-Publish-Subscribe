#ifndef REQUESTS_H
#define REQUESTS_H

struct RcvMsg;
struct SndMsg;
struct ServerInfo;

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

int doRequest_Login(struct ServerInfo*, struct SndMsg*);
int doRequest_Logout(struct ServerInfo*, struct SndMsg*);
int doRequest_Register(struct ServerInfo*, struct RcvMsg*);
int doRequest_Unregister(struct ServerInfo*, struct RcvMsg*);
int doRequest_NewType(struct ServerInfo*, struct SndMsg*);
int doRequest_RandomType(struct ServerInfo*, struct SndMsg*);
int doRequest_SendMsg(struct ServerInfo*, struct SndMsg*);
int doRequest_GetMsg(struct ServerInfo*, struct RcvMsg*);
int doRequest_GetTypes_sender(struct ServerInfo*, struct SndMsg*);
int doRequest_GetTypes_receiver(struct ServerInfo *server, struct RcvMsg *rcvMsg);

#endif // REQUESTS_H
