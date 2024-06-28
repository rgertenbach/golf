#!/bin/sh

cc -o golf_test golf_test.c golf.c -Wall -Wextra
./golf_test
rm golf_test
