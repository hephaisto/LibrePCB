TEMPLATE = lib
TARGET = pylibrepcb

# Set the path for the generated binary
GENERATED_DIR = ../../../generated

# Use common project definitions
include(../../../common.pri)

QT += core widgets network sql opengl printsupport

# this is needed for cases with version numbers like 3.5m like available on debian
#PYTHON_VERSION_MINOR_DIGITONLY = $$find(PYTHON_VERSION_MINOR, "la")
#message($$PYTHON_VERSION_MINOR_DIGITONLY)


#QMAKE_TARGET

CONFIG += staticlib

INCLUDEPATH += \
    ../../

SOURCES += \
    pylibrepcb.cpp \
    qstring_converters.cpp \
    core.cpp \
    geometry.cpp \
    symbol.cpp \
    package.cpp \
    embedding.cpp \

HEADERS += \
    qstring_converters.h \
    core.h \
    geometry.h \
    symbol.h \
    package.h \
    embedding.h \


FORMS += \

LIBS += \
    -llibrepcbworkspace \
    -llibrepcbproject \
    -llibrepcblibrary \
    -llibrepcbcommon \
    -lsexpresso \
    -lboost_python-py3$$PYTHON_VERSION_MINOR \
    -lpython3.$$PYTHON_VERSION_MINOR \


DEFINES += BOOST_PYTHON_DYNAMIC_LIB

unix {
    QMAKE_CXXFLAGS += \
        "$$system(python3.$$PYTHON_VERSION_MINOR-config --cflags)"

    QMAKE_LFLAGS += \
        "$$system(python3.$$PYTHON_VERSION_MINOR-config --ldflags)" \
        "-L$$DESTDIR" \
    QMAKE_POST_LINK += ln -f -s libpylibrepcb.so $$GENERATED_DIR/unix/librepcb.so
}
