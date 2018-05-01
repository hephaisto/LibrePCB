#!/usr/bin/env bash

# set shell settings (see https://sipb.mit.edu/doc/safe-shell/)
set -eufv -o pipefail

# make all warnings into errors
CFLAGS="-Werror"
CXXFLAGS="-Werror"

# set special flag for clang (see https://github.com/travis-ci/travis-ci/issues/5383)
if [ "$CC" = "clang" ]; then CFLAGS+=" -Qunused-arguments"; fi
if [ "$CXX" = "clang++" ]; then CXXFLAGS+=" -Qunused-arguments"; fi

QMAKE_CMDLINE_FLAGS=""
if [ -z "$PYTHON_VERSION_TO_LINK" ];then QMAKE_CMDLINE_FLAGS="PYTHON_VERSION=$PYTHON_VERSION_TO_LINK"; fi

# build librepcb
mkdir build
cd build
qmake ../librepcb.pro -r "QMAKE_CXX=$CXX" "QMAKE_CC=$CC" "QMAKE_CFLAGS=$CFLAGS" "QMAKE_CXXFLAGS=$CXXFLAGS" $QMAKE_CMDLINE_FLAGS
make -j8

