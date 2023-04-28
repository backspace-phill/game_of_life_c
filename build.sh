#! /bin/env bash

OUTPUT_NAME="cgol"
CFLAGS="-DNDEBUG -O3 -o "$OUTPUT_NAME
CCOMP=cc
INFILE=./cgol_in_c.c

rm cgol

echo "---BUILDING---"
$CCOMP $INFILE $CFLAGS
