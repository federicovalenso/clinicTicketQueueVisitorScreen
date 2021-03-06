QT += core gui multimedia quick svg

QMAKE_CXXFLAGS += -O2
CONFIG += c++1z
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
    ticketmodel.cpp \
    ticketprocessor.cpp \
    appsettings.cpp \
    ticketnumberparser.cpp

RESOURCES += \
    sounds.qrc \
    gui.qrc

HEADERS += \
    ticketsplayer.h \
    ticketmodel.h \
    ticket.h \
    ticketprocessor.h \
    appsettings.h \
    ticketnumberparser.h

android {
    QT += androidextras
}

win32:RC_ICONS += icon.ico

DISTFILES += \
    android-sources/AndroidManifest.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources
