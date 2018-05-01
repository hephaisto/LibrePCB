TEMPLATE = lib
TARGET = pylibrepcb

CONFIG += staticlib

GENERATED_DIR = ../../../generated

include(../../../common.pri)

QT += core widgets

INCLUDEPATH += \
    ../../

SOURCES += \
    embedding.cpp \

HEADERS += \
    embedding.h \

OBJECTS_DIR = .tmpstub
