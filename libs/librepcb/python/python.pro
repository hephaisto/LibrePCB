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

PYTHON_VERSIONS = $$split(PYTHON_VERSION, ".")
PYTHON_VERSION_MAJOR = $$member(PYTHON_VERSIONS, 0)
PYTHON_VERSION_MINOR = $$member(PYTHON_VERSIONS, 1)
!equals(PYTHON_VERSION_MAJOR, "3") {
    error("Only python 3.x is supported, but major version was $$PYTHON_VERSION_MAJOR")
}
message(Linking against python3.$$PYTHON_VERSION_MINOR)


#QMAKE_TARGET

CONFIG += dynamiclib

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

QMAKE_CXXFLAGS += \
    "$$system(python3.$$PYTHON_VERSION_MINOR-config --cflags)"

QMAKE_LFLAGS += \
    "$$system(python3.$$PYTHON_VERSION_MINOR-config --ldflags)" \
    "-L$$DESTDIR" \

DEFINES += BOOST_PYTHON_DYNAMIC_LIB

unix {
    QMAKE_POST_LINK += ln -f -s libpylibrepcb.so $$GENERATED_DIR/unix/librepcb.so
}
