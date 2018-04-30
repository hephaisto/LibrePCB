
# Set the path for the generated binary
GENERATED_DIR = ../../../generated

# Use common project definitions
include(../../../common.pri)

QT += core widgets network sql opengl printsupport

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
}
