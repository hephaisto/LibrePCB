TEMPLATE = subdirs

SUBDIRS = \
    common \
    library \
    project \
    workspace \
    eagleimport \
    libraryeditor \
    projecteditor \
    librarymanager \
    python \

library.depends = common
project.depends = library
workspace.depends = project
eagleimport.depends = workspace
libraryeditor.depends = eagleimport python
projecteditor.depends = eagleimport
librarymanager.depends = libraryeditor
