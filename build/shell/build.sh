#!/bin/bash
FILES=`find ../.. -iname *.cxx`
echo $FILES

c++ -Wfatal-errors -I../msvc/Tests -I../../include $FILES -o test
