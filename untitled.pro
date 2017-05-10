QT += core
QT -= gui

TARGET = untitled
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

LIBS += -lusb-1.0

INCLUDEPATH = /usr/include/libusb-1.0

HEADERS += \
    dev.h
