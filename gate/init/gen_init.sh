#!/bin/sh

BASE_DIR=$(dirname "$(readlink -f "$0")")
TMP_DIR=`mktemp -d`
INIT_DIR="$TMP_DIR/init"

mkdir $INIT_DIR

for f in "init.ino" "../id.c" "../id.h"; do
    ln -s "$BASE_DIR/$f" "$INIT_DIR/$(basename $f)"
done

arduino $TMP_DIR/init/init.ino

wait; rm $TMP_DIR
