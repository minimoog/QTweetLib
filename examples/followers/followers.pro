QT       += core gui network declarative

TARGET = followers
TEMPLATE = app
INCLUDEPATH += ../../src

SOURCES += \
    main.cpp \
    followers.cpp \
    followerslistmodel.cpp

HEADERS += \
    followers.h \
    followerslistmodel.h

FORMS += \
    followers.ui

OTHER_FILES += \
    FollowerDelegate.qml \
    FollowersList.qml

RESOURCES += \
    followers.qrc

symbian: LIBS += -lqtweetlib
else:unix|win32: LIBS += -L$$OUT_PWD/../../lib/ -lqtweetlib

INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../lib
