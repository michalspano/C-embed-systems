#!/bin/sh

# Upload sketch to the board.
cd main; arduino-cli upload -p /dev/cu.usbmodemF412FA651FA82 --fqbn arduino:renesas_uno:unor4wifi
