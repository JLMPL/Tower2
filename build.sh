#!/bin/sh

export CXX=/usr/bin/g++-7

cmake .
make -j4
