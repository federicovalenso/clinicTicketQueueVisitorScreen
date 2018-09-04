QT += core gui widgets multimedia

QMAKE_CXXFLAGS += -O0
CONFIG += c++11

TARGET = TicketVoice
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    ticketsplayer.cpp \
    mainwindow.cpp \
    ticketmodel.cpp \
    ticketprocessor.cpp \
    appsettings.cpp

RESOURCES += \
    sounds.qrc

HEADERS += \
    ticketsplayer.h \
    mainwindow.h \
    ticketmodel.h \
    ticket.h \
    ticketprocessor.h \
    appsettings.h

FORMS += \
    mainwindow.ui

DISTFILES +=
