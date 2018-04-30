TEMPLATE = lib
TARGET = pylibrepcb

CONFIG += dynamiclib
include(common.pri)

OBJECTS_DIR = .tmpdynamic

unix {
    QMAKE_POST_LINK += ln -f -s libpylibrepcb.so $$GENERATED_DIR/unix/librepcb.so
}
