#!/bin/bash
FILES=`find ../.. -iname *.cxx`
echo $FILES

c++ -std=c++03 -Wfatal-errors -I../msvc/Tests -I../../include $FILES -o test
