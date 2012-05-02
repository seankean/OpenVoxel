#!/bin/sh
sudo ./i2c_send /dev/i2c-2   4 0 0 0 0
sleep 5
sudo ./i2c_send /dev/i2c-2   5 0 0 0 7
sleep .5
sudo ./i2c_send /dev/i2c-2 130 0 0 0 6
sleep 5
sudo ./i2c_send /dev/i2c-2 146 0 0 0 0
sleep .5
sudo ./i2c_send /dev/i2c-2  38 0 0 0 0
sleep 5
sudo ./i2c_send /dev/i2c-2  98 0 0 0 0
sleep .5
sudo ./i2c_send /dev/i2c-2  36 0 0 0 1
sleep 5
sudo ./i2c_send /dev/i2c-2  31 0 0 0 9
sleep .5

