#!/bin/sh
./i2c_send /dev/i2c-1 4 0 0 0 1
sleep 5
./i2c_send /dev/i2c-1 4 0 0 0 0
sleep .5
