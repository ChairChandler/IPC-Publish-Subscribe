Structure description:



Bool - enumerator, C++ bool equivalent:
  - False - false,
  - True - true



Error - enumeration type for selecting error type on the server size.

Error types:
  - MaxLoggedUsers - maximum logged persons,
  - MaxRegisteredUsers - maximum registered persons,
  - MaxMsgs - maximum messages number in user message box,
  - NoMsg - no messages in user message box,
  - NoLoggedUser - user not logged,
  - NoRegisteredUser - user not registered,
  - LoggedUser - user logged,
  - RegisteredUser - user registered,
  - WrongMsgType - wrong message type; type had been registered in a system,
  - NoMsgTypes - no message type in a system,
  - MaxMsgTypes - maximum message type in a system,
  - WrongID - invalid sender ID who is logged out




Port - enumeration type for selecting port which will be used.

Types of ports:
a) server ports:
  - Receivers_port - the port on which the server receives messages from recipients,
  - Senders_port - the port on which the server receives messages from senders

b) client ports:
  - Message_port - the port on which the client receives messages from the server, both text and various types of data, e.g. message types, random type etc. (general purpose port)
  - Error_port - the port on which error messages from the server come,
  - Notification_port - the port on which notifications from the server come about a new message in the user's container,
  - Async_port - the port on which messages from the server arrive using the asynchronous method,
  - Answer_port - the port on which the client can check whether the server is active





Message - structure containing information about the message, ie the content, priority and type of the message.
The message type is used to specify recipients who have signed up to receive a given type of message.







Wrapper - a structure used to package the selected data type from the Wrapper_data union.




Wrapper_data - union used to choose one of the following elements:
  - Error error - error,
  - RcvMsg rcvMsg - messages from or to the recipient,
  - SndMsg sndMsg - messages from or to the sender,
  - Bool isNotification - new message notifications,
  - char emptyData - an empty message sent on the client's Answer_port port (can not send a message without any type of data, therefore this is the reason for creating this variable)



ReceiverInfo - structure describing the recipient:
  - **char name [BUFF_SIZE]** - name,
  - **MsgType rcvMsgTypes [BUFF_SIZE]** - interesting types of messages to receive,
  - RcvInfoNotifications rcvNot - decision on how to notify,
  - RcvInfoSubscription rcvSub - the decision about choosing the subscription method,
  - RcvInfoSend rcvSend - the decision about choosing the method of message retrieval





RcvInfoSend - enumerator, way of message reception:
  - Manual - manual, manual reception of messages,
  - Automatic - automatic, sending immediately after receiving an interesting message by the server




RcvInfoSubscription - enumerator, subscription method:
  - TemporarySubscription - temporary subscription, after a set time the user will be removed from registered persons, but will not be logged out
  - PermanentSubscription - permanent subscription without removal




RcvInfoNotifications - enumerator, way of notification:
  - NoNotifications - without notifications,
  - Notifications - with notification, after detecting an interesting message, the server sends a notification about the new message




RcvMsg - a structure for packaging the recipient's request with data sent to the server or only data sent to the recipient from the server:
  - ClientRequest request - user request,
  - RcvMsg_Data data - selected data type to send



RcvMsg_Data - union used to select one of the following items to send:
  - ReceiverInfo user - user information (recipient -> server),
  - Message msg - message (recipient <- server),
  - MsgType msgTypeArray [BUFF_SIZE] - message types currently on the server (recipient <- server)




ClientRequest - enumerator, client requests:
a) only the recipient:
  - Register,
  - Unregister,
  - GetMsg,

b) only the sender:
  - Login - log in to the server,
  - Logout - logout from the server,
  - NewType - send a new type created by the server upload,
  - RandomType - request creation of a random type,
  - SendMsg - send a message to the server

c) both:
  - GetTypes - download types from the server





SenderInfo - structure describing the sender:
  - int ID - sender ID,
  - char name [BUFF_SIZE] - name




SndMsg - a structure used to package the sender's request with data sent to the server or only data sent to the sender from the server:
- ClientRequest request - sender's request,
- SenderInfo user - information about the sender
- SndMsg_Data data - selected data type to send




SndMsg_Data - union used to select one of the following items to send:
- MsgType msgType - message type created by the sender (sender -> server)
- MsgType msgTypesArray [BUFF_SIZE] - message types currently on the server (sender <- server)
- Message msg - message (sender -> server)





ServerInfo - structure, information about the server (program):
- int fileDesc - server file descriptor,
- RegisterUserInfo registeredUsers [BUFF_SIZE] - information board of registered users,
- SenderInfo loggedUsers [BUFF_SIZE] - information table of logged in users,
- MsgType msgTypes [BUFF_SIZE] - current data types on the server





ClientInfo - structure, information about the client (program):
- int clientFileDesc - client file descriptor,
- int serverFileDesc - server file descriptor,
- Error * lastError - last error encountered





ListeningThreadsInfo - a structure for describing and controlling the operation of listening threads:
- guint notificationThread - the number of the thread listening to the notification,
- guint messageThread - the thread number of the listening message,
- volatile Bool stopNotThread - information about stopping the notification thread,
- volatile Bool stopMessageThread - information about stopping the message thread
















=======================================
Description of the functionality of the components:
a) the customer




The main.c, logicwindow.c, and registerwindow.c files are used to control the GUI in GTK.

The msgbox.c file contains definitions of functions that allow displaying the message dialog (error, success).

The file listening.c contains definitions of functions called in registerwind.c. They are used to wait for a new message, notification depending on the user preferences selected in main.c in the options window; displaying a new message, notifications. They are interrupted after receiving the message of a new person created (new registration) or finalizing the program.

The file connect.c contains definitions of functions used to send user requests, they are called in logicwindow.c and registerwindow.c.
Also included are auxiliary functions such as checking a new message or notification (used in listening.c).




b) server


The connect.c file initiates server work and waits for new requests in the meantime by sending new notifications (if they are) and if the user has selected temporary subscriptions and has exceeded the time for temporary registration, it will be removed from registered users (will not be logged out). If a request is detected, the corresponding functions are called in the request.c file


The file server.c contains definitions of basic, non-abstract auxiliary functions that operate directly on arrays. Thanks to them, the request.c module can work properly.
