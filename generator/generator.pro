#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T20:50:03
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = generator
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    generator.cpp

OTHER_FILES += \
    twitter_api.json

HEADERS += \
    generator.h
