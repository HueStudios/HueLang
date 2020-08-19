#!/usr/bin/env bash
valgrind --trace-children=yes env LD_LIBRARY_PATH=./lib/ bin/huelang
