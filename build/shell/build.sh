#!/bin/bash
FILES=`find ../.. -iname *.cxx`
echo $FILES

clang -std=c++03 -Wfatal-errors -I/usr/local/include -I../msvc/Tests -I../../include $FILES -o test

