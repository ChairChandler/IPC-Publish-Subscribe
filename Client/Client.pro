TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

CONFIG += link_pkgconfig
PKGCONFIG += gtk+-3.0


SOURCES += \
    connect.c \
    main.c \
    message.c \
    msgtype.c \
    receivers.c \
    senders.c \
    loginwindow.c \
    registerwindow.c \
    msgbox.c \
    removewidgets.c \
    listening.c

HEADERS += \
    bool.h \
    buffsize.h \
    client.h \
    connect.h \
    error.h \
    loginwindow.h \
    message.h \
    msgbuf.h \
    msgtype.h \
    receivers.h \
    receiversmsg.h \
    registerwindow.h \
    requests.h \
    senders.h \
    sendersmsg.h \
    msgbox.h \
    removewidgets.h \
    listening.h \
    port.h
