TEMPLATE = lib
TARGET = librepcbpy

# Set the path for the generated binary
GENERATED_DIR = ../../../generated

# Use common project definitions
include(../../../common.pri)

QT += core widgets network sql opengl printsupport

#QMAKE_TARGET

CONFIG += dynamiclib

INCLUDEPATH += \
    ../../

SOURCES += \
    librepcbpy.cpp \
    qstring_converters.cpp \
    core.cpp \
    geometry.cpp \
    symbol.cpp \
    package.cpp \
    component.cpp \
    device.cpp \
    embedding.cpp \

HEADERS += \
    qstring_converters.h \
    core.h \
    geometry.h \
    symbol.h \
    package.h \
    component.h \
    device.h \
    embedding.h \


FORMS += \

LIBS += \
	-llibrepcbworkspace \
	-llibrepcbproject \
	-llibrepcblibrary \
	-llibrepcbcommon \
	-lsexpresso \
	-lboost_python-py35 \

QMAKE_CXXFLAGS += \
	"$$system(python3-config --cflags)"

QMAKE_LFLAGS += \
	"$$system(python3-config --ldflags)" \
	"-L$$DESTDIR" \

DEFINES += BOOST_PYTHON_DYNAMIC_LIB
