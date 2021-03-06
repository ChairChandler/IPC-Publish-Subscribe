Structure description
=======================


- Bool - enumerator, C++ bool equivalent
  - **False** - false
  - **True** - true



- Error - enumeration type for selecting error type on the server size
  - **MaxLoggedUsers** - maximum logged persons
  - **MaxRegisteredUsers** - maximum registered persons
  - **MaxMsgs** - maximum messages number in user message box
  - **NoMsg** - no messages in user message box
  - **NoLoggedUser** - user not logged
  - **NoRegisteredUser** - user not registered
  - **LoggedUser** - user logged
  - **RegisteredUser** - user registered
  - **WrongMsgType** - wrong message type; type had been registered in a system
  - **NoMsgTypes** - no message type in a system
  - **MaxMsgTypes** - maximum message type in a system
  - **WrongID** - invalid sender ID who is logged out




- Port - enumeration type for selecting port which will be used
  - server ports
    - **Receivers_port** - the port on which the server receives messages from recipients
    - **Senders_port** - the port on which the server receives messages from senders

  - client ports
    - **Message_port** - the port on which the client receives messages from the server, both text and various types of data, e.g. message types, random type etc. (general purpose port)
    - **Error_port** - the port on which error messages from the server come
    - **Notification_port** - the port on which notifications from the server come about a new message in the user's container
    - **Async_port** - the port on which messages from the server arrive using the asynchronous method
    - **Answer_port** - the port on which the client can check whether the server is active





- Message - structure containing information about the message, ie the content, priority and type of the message.
The message type is used to specify recipients who have signed up to receive a given type of message.







- Wrapper - a structure used to package the selected data type from the Wrapper_data union.
  - Wrapper_data - union used to choose one of the following elements
    - **error** - error
    - **rcvMsg** - messages from or to the recipient
    - **sndMsg** - messages from or to the sender
    - **isNotification** - new message notifications
    - **emptyData** - an empty message sent on the client's Answer_port port (can not send a message without any type of data, therefore this is the reason for creating this variable)



- ReceiverInfo - structure describing the recipient
  - **name** - name
  - **rcvMsgTypes** - interesting types of messages to receive
  - **rcvNot** - decision on how to notify
  - **rcvSub** - the decision about choosing the subscription method
  - **rcvSend** - the decision about choosing the method of message retrieval





- RcvInfoSend - enumerator, way of message reception
  - **Manual** - manual, manual reception of messages
  - **Automatic** - automatic, sending immediately after receiving an interesting message by the server




- RcvInfoSubscription - enumerator, subscription method
  - **TemporarySubscription** - temporary subscription, after a set time the user will be removed from registered persons, but will not be logged out
  - **PermanentSubscription** - permanent subscription without removal




- RcvInfoNotifications - enumerator, way of notification
  - **NoNotifications** - without notifications
  - **Notifications** - with notification, after detecting an interesting message, the server sends a notification about the new message



- RcvMsg - a structure for packaging the recipient's request with data sent to the server or only data sent to the recipient from the server
  - **request** - user request
  - **data** - selected data type to send


- RcvMsg_Data - union used to select one of the following items to send
  - **user** - user information (recipient -> server)
  - **msg** - message (recipient <- server)
  - **msgTypeArray** - message types currently on the server (recipient <- server)



- ClientRequest - enumerator, client requests
  - only the recipient
    - **Register**
    - **Unregister**
    - **GetMsg**

  - only the sender
    - **Login** - log in to the server
    - **Logout** - logout from the server
    - **NewType** - send a new type created by the server upload
    - **RandomType** - request creation of a random type
    - **SendMsg** - send a message to the server

  - both
    - **GetTypes** - download types from the server





- SenderInfo - structure describing the sender
  - **ID** - sender ID
  - **name** - name




- SndMsg - a structure used to package the sender's request with data sent to the server or only data sent to the sender from the server
  - **request** - sender's request
  - **user** - information about the sender
  - **data** - selected data type to send




- SndMsg_Data - union used to select one of the following items to send
  - **msgType** - message type created by the sender (sender -> server)
  - **msgTypesArray** - message types currently on the server (sender <- server)
  - **msg** - message (sender -> server)





- ServerInfo - structure, information about the server (program)
  - **fileDesc** - server file descriptor
  - **registeredUsers** - information board of registered users
  - **loggedUsers** - information table of logged in users
  - **msgTypes** - current data types on the server





- ClientInfo - structure, information about the client (program)
  - **clientFileDesc** - client file descriptor
  - **serverFileDesc** - server file descriptor
  - **lastError** - last error encountered





- ListeningThreadsInfo - a structure for describing and controlling the operation of listening threads
  - **notificationThread** - the number of the thread listening to the notification
  - **messageThread** - the thread number of the listening message
  - **stopNotThread** - information about stopping the notification thread
  - **stopMessageThread** - information about stopping the message thread


Description of the functionality of the components
=======================

- Client




The main.c, logicwindow.c, and registerwindow.c files are used to control the GUI in GTK.

The msgbox.c file contains definitions of functions that allow displaying the message dialog (error, success).

The file listening.c contains definitions of functions called in registerwind.c. They are used to wait for a new message, notification depending on the user preferences selected in main.c in the options window; displaying a new message, notifications. They are interrupted after receiving the message of a new person created (new registration) or finalizing the program.

The file connect.c contains definitions of functions used to send user requests, they are called in logicwindow.c and registerwindow.c.
Also included are auxiliary functions such as checking a new message or notification (used in listening.c).




- Server


The connect.c file initiates server work and waits for new requests in the meantime by sending new notifications (if they are) and if the user has selected temporary subscriptions and has exceeded the time for temporary registration, it will be removed from registered users (will not be logged out). If a request is detected, the corresponding functions are called in the request.c file


The file server.c contains definitions of basic, non-abstract auxiliary functions that operate directly on arrays. Thanks to them, the request.c module can work properly.
