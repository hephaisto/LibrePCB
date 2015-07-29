# LibrePCB [![Build Status](https://travis-ci.org/LibrePCB/LibrePCB.svg?branch=master)](https://travis-ci.org/LibrePCB/LibrePCB)

## About LibrePCB

LibrePCB is a free Schematic / Layout Editor to develop printed circuit boards.
It runs on Linux, Windows and Mac. The project is still in a quite early
development stage (no stable release available).

![Screenshot](doc/screenshot.png)

## Development

### Requirements

To compile LibrePCB, you need the following software components:
- g++ >= 4.8, MinGW >= 4.8, or Clang >= 3.3 (C++11 support is required)
- Qt >= 5.4 (http://www.qt.io/download-open-source/)
- libglu1-mesa-dev (`sudo apt-get install libglu1-mesa-dev`)

### Building

#### Using qtcreator

Building with [qtcreator](http://doc.qt.io/qtcreator/) is probably the easiest
way.  To keep build time as low as possible make sure to set the correct make
flags to use all available CPU cores to build. See this [stackoverflow
answer](https://stackoverflow.com/questions/8860712/setting-default-make-options-for-qt-creator).

#### Using qmake and make

Since qtcreator is also using qmake and make to build, it's easy to do the same
on the command line:

```bash
$ mkdir build && cd build
$ qmake -r ../librepcb.pro
$ make -j 8
```

Then grab a coffee, since the compilation takes about two minutes on a decent
laptop:
```
make -j8  819.54s user 44.92s system 726% cpu 1:58.93 total
```

### Documentation

- Doxygen: http://librepcb.github.io/LibrePCB-Doxygen/master/
- Some diagrams (partially outdated): https://github.com/LibrePCB/LibrePCB/tree/master/dev/diagrams/svg

### Workspace

At the first startup, LibrePCB asks for a workspace directory where the library
elements and projects will be saved.  For developers there is a demo workspace
inclusive library and projects in this repository (subdirectory "dev/workspace/").

## License

LibrePCB is published under the [GNU GPLv3](http://www.gnu.org/licenses/gpl-3.0.html) license.
