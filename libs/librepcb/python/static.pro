TEMPLATE = lib
TARGET = pylibrepcb

CONFIG += staticlib
include(common.pri)

OBJECTS_DIR = .tmpstatic

QMAKE_CXXFLAGS += -DPYTHON_DO_STANDALONE_INITIALISATION
