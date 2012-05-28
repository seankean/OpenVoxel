#!/usr/bin/python

import serial


print "trying to connect to serial port...\n"
ser = serial.Serial('/dev/ttyACM0', 115200)

print "\nconnected.\n"
print ser.read()
print ser.read()
ser.write('A')

for i in range(1, 100):
    print ser.read()

