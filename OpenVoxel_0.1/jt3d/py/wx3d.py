#!/usr/bin/python
import wx
from serial import Serial

class MyFrame(wx.Frame):
    def __init__(self, parent, id, title):

        # Initialize the serial port.
        self.ser = Serial('/dev/ttyACM0', 115200)
        self.ser.read()
        self.ser.read()
        self.ser.write('A')

        # Initialize the window.
        wx.Frame.__init__(self, parent, id, title, size = (320, 320))
        self.car = wx.Bitmap('car.jpeg')
        self.boat = wx.Bitmap('boat.jpeg')
        self.bitmap = self.boat
        wx.EVT_PAINT(self, self.OnPaint)
        self.Centre()

        # Initialize the timer.
        TIMER_ID = 100  # pick a number
        self.timer = wx.Timer(self, TIMER_ID)  # message will be sent to the panel
        self.timer.Start(4)  # x100 milliseconds
        wx.EVT_TIMER(self, TIMER_ID, self.on_timer)  # call the on_timer function


    def OnPaint(self, event):
        dc = wx.PaintDC(self)
        dc.DrawBitmap(self.bitmap, 20, 20)

    def on_timer(self, event):
        # Read the serial port.
        current_state = self.ser.read()
        print current_state
        if current_state == 'B': self.bitmap = self.boat
        else: self.bitmap = self.car
        self.Refresh()
        pass  # do whatever


class MyApp(wx.App):
    def OnInit(self):
        frame = MyFrame(None, -1, 'car')
        frame.Show(True)
        self.SetTopWindow(frame)
        return True

    def OnDestroy(self):
        self.ser.close()
        return True


app = MyApp(0)
app.MainLoop()
