QT += testlib
QT += gui
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  \
    ../ticketnumberparser.cpp \
    ticketsplayertest.cpp

HEADERS += \
    ../ticketnumberparser.h \
    ticketsplayertest.h
