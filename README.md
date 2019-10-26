# **IPC Publish Subscribe** ![Build_status](https://travis-ci.org/ChairChandler/IPC-Publish-Subscribe.svg?branch=master)


[Publish–subscribe](https://en.wikipedia.org/wiki/Publish%E2%80%93subscribe_pattern) is a messaging pattern where senders of messages, called publishers, do not program the messages to be sent directly to specific receivers, called subscribers, but instead categorize published messages into classes without knowledge of which subscribers, if any, there may be. Similarly, subscribers express interest in one or more classes and only receive messages that are of interest, without knowledge of which publishers, if any, there are.

[Interprocess communication (IPC)](https://en.wikipedia.org/wiki/Inter-process_communication) refers specifically to the mechanisms an operating system provides to allow the processes to manage shared data. Typically, applications can use IPC, categorized as clients and servers, where the client requests data and the server responds to client requests.


**Compilation instruction**
=======================

In every directories there is a Makefile, in this directory you ought to run "make" command.


**Start-up instruction**
=======================

To run server you have to run "Server" file in "Server" directory.
To run client you have to run "Client" file in "Client" directory.


**C files content**
======================
###### Server

  - main.c	
    - Contains server IP and start-up function "run".

  - connect.c 
    - Main file for managing communication, it initialize server and run infinite loop, reads sender requests and receives and sends asynchronous messages, checks subscription time and managed it.

  - request.c
    - Contains request system handler.

  - server.c
    - Contains basic functions for searching, updating, deleting, adding expected elements of senders, recipients, messages etc.

  - message.c
    - Contains definition of empty message.

  - msgtype.c
    - Contains definition of empty message type.

  - receivers.c
    - Contains definition of empty receiver.

  - senders.c
    - Contains definition of empty sender.

--------------------------
###### Client
  - main.c
    - Contains intialize GTK application, managed main menu GUI, register GUI, login GUI, register option GUI. 
  Also contains signals for managing registering option window.

  - loginwindow.c
    - Contains logging window signals.

  - registerwindow.c
    - Contains registering window signals.

  - listening.c
    - Contains definition of threads functions for listening and creating messages dialogs with new message or new notification.

  - connect.c
    - Contains definitions of functions that perform client requests, but also auxiliary functions such as checking if there is a new notification.

  - msgbox.c
    - Contains definition of functions for creates and shows messages dialog in type error or succes.

  - removewidgets.c
    - Contains definition of removal widgets function.

  - message.c
    - Contains definition of empty message.

  - msgtype.c
    - Contains definition of empty message type.

  - receivers.c
    - Contains definition of empty receiver.

  - senders.c
    - Contains definition of empty sender.
