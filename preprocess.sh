#!/bin/sh

set -eu

SKETCH_DIR=$1
DESTINATION_DIR="${2:-./backend/executable}"/sketch
TARGET_FILE=$DESTINATION_DIR/sketch.cpp

mkdir -p $DESTINATION_DIR
cp $SKETCH_DIR/* $DESTINATION_DIR

arduino-cli compile -b arduino:avr:uno --preprocess "$SKETCH_DIR" > "$TARGET_FILE"
rm $DESTINATION_DIR/*.ino

echo "Preprocessing succesful"
echo "$(realpath "$TARGET_FILE") created"