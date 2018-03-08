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

# load boost
_BOOST_ROOT = $$(BOOST_ROOT)
isEmpty(_BOOST_ROOT) {
    message(warning: `BOOST_ROOT` environment variable not defined)
}
else
{
    INCLUDEPATH += $$_BOOST_ROOT
}

SOURCES += \
    librepcbpy.cpp \
    qstring_converters.cpp \
    core.cpp \
    geometry.cpp \
    library.cpp \
    embedding.cpp \

HEADERS += \
    qstring_converters.h \
    core.h \
    geometry.h \
    library.h \
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
