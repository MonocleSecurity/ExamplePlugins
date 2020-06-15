#!/bin/bash

gcc -fPIC -c plugin.c
gcc -shared -o plugin.so plugin.o
zip plugin.zip plugin.so
