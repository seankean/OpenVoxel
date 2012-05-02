#!/usr/bin/python
import serial
import Tkinter, random


class App:
    def __init__(self, t):
        # Initialize the serial port.
        ser = serial.Serial('/dev/ttyACM0', 115200)
        print ser.read()
        print ser.read()
        ser.write('A')

        # Read from the serial port.
        while True:
            print ser.read()

            self.i = Tkinter.PhotoImage(width=100,height=100)
            #colors = [[random.randint(0,255) for i in range(0,3)] for j in range(0,10000)]
            #row = 0; col = 0
            #for color in colors:
            #   self.i.put('#%02x%02x%02x' % tuple(color),(row,col))
            #   col += 1
            #   if col == 100:
            #       row +=1; col = 0        
            c = Tkinter.Canvas(t, width=100, height=100); c.pack()
            c.create_image(0, 0, image = self.i, anchor=Tkinter.NW)

t = Tkinter.Tk()
a = App(t)    
t.mainloop()

