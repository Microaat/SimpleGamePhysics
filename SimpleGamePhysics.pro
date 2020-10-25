TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -L/usr/local/lib -lSDL2

SOURCES += main.cpp \
    Game.cpp \
    point.cpp

QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    Game.h \
    particle.h \
    point.h \
    velocity.h

