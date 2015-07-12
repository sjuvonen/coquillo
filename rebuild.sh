#!/usr/bin/sh
make distclean
qmake CONFIG+=c++11
make -j7

