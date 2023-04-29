#!/bin/bash

# Functions

time-library() {
    library=$1
    printf "  Timing %-31s ... " $library
    if diff -y <(env LD_PRELOAD=./lib/$library ./bin/test_07 $library 2> /dev/null) <($library-output) >& test.log; then
    	echo "Success"
    else
    	echo "Failure"
    	cat test.log
    	echo ""
    fi
}

# Main execution

time-library libmalloc-ff.so
time-library libmalloc-bf.so
time-library libmalloc-wf.so

# vim: sts=4 sw=4 ts=8 ft=sh
