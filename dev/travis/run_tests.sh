#!/usr/bin/env bash

# set shell settings (see https://sipb.mit.edu/doc/safe-shell/)
set -eufv -o pipefail

# run tests
if [ "${TRAVIS_OS_NAME}" = "linux" ]
then
  xvfb-run -a ./build/generated/unix/qztest
  xvfb-run -a ./build/generated/unix/tests
elif [ "${TRAVIS_OS_NAME}" = "osx" ]
then
  ./build/generated/mac/qztest
  ./build/generated/mac/tests
fi

# run python library tests
if [ -z "${PYTHON_VERSION_TO_LINK}" ]
then
    # use empty default PYTHONPATH
    if [ -z ${PYTHONPATH+x} ]
    then
        PYTHONPATH=""
    fi

    PYTHONPATH=$PYTHONPATH:./build/generated/unix python3 -m unittest discover -v -s ./tests/python
fi
