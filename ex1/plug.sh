#!/bin/sh

# Upload sketch to the board. Used when testing the sketch with the physical components.
cd main; arduino-cli upload -p /dev/cu.usbmodemF412FA651FA82 --fqbn arduino:renesas_uno:unor4wifi
