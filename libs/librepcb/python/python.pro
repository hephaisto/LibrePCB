TEMPLATE = lib
TARGET = librepcbsexpression

# Set the path for the generated binary
GENERATED_DIR = ../../../generated

# Use common project definitions
include(../../../common.pri)

QT += core widgets

CONFIG += dynamiclib

INCLUDEPATH += \
    ../../

SOURCES += \
    sexpression.cpp \

HEADERS += \


FORMS += \

LIBS += \
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
