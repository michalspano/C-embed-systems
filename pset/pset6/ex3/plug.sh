#!/bin/sh

# A simple POSIX-sh compliant script to compile and upload the sketch to the
# board (this is helpful when a more light-weight option than Arduino IDE is
# used).

SRC="./main"                  # relative path to the source
BOARD="arduino:avr:uno"       # Plain Arduino Uno
PORT="/dev/cu.usbserial-1140" # At this destination (may vary per system)

arduino-cli compile --fqbn $BOARD $SRC         # compile
arduino-cli upload -p $PORT --fqbn $BOARD $SRC # upload

# If -p flag is given, open the serial monitor
if [ "$1" = "-p" ]; then cat $PORT; fi
