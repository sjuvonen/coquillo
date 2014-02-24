#!/usr/bin/sh
make distclean
qmake QMAKE_CXXFLAGS+=-std=c++11
make -j7

