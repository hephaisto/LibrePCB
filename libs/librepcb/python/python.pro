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
_BOOST_INCLUDEDIR = $$(BOOST_INCLUDEDIR)
isEmpty(_BOOST_INCLUDEDIR) {
    message(warning: `BOOST_INCLUDEDIR` environment variable not defined)
}
else
{
    INCLUDEPATH += $$_BOOST_INCLUDEDIR
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

unix {
    QMAKE_CXXFLAGS += \
        "$$system(python3-config --cflags)"

    QMAKE_LFLAGS += \
        "$$system(python3-config --ldflags)" \
        "-L$$DESTDIR" \
}

DEFINES += BOOST_PYTHON_DYNAMIC_LIB
