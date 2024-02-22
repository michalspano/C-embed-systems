#!/bin/sh

# A simple POSIX-sh compliant script to compile and upload the sketch to the
# board (this is helpful when a more light-weight option than Arduino IDE is
# used).

SRC="./main"                          # relative path to the source
BOARD="arduino:renesas_uno:unor4wifi" # Arduino Uno Rev. 4 Wifi
PORT="/dev/cu.usbmodemF412FA651FA82"  # may be adjusted based on the machine

arduino-cli compile --fqbn $BOARD $SRC         # compile
arduino-cli upload -p $PORT --fqbn $BOARD $SRC # upload
