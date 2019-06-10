TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c \
    receivers.c \
    senders.c \
    message.c \
    server.c \
    connect.c \
    msgtype.c \
    requests.c

HEADERS += \
    connect.h \
    message.h \
    receivers.h \
    senders.h \
    requests.h \
    server.h \
    bool.h \
    sendersmsg.h \
    receiversmsg.h \
    msgtype.h \
    buffsize.h \
    error.h \
    msgbuf.h \
    port.h
