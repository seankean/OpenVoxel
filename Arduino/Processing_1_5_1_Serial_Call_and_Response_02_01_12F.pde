
//Processing sketch to run with this example:
// This example code is in the public domain.

import processing.serial.*;

int bgcolor;                 // Background color
int fgcolor;                 // Fill color
Serial myPort;                       // The serial port
int[] serialInArray = new int[3];    // Where we'll put what we receive
int serialCount = 0;                 // A count of how many bytes we receive
int xpos, ypos;                  // Starting position of the ball
boolean firstContact = false;        // Whether we've heard from the microcontroller

// additional variables
int byte1 = 1;
int byte2 = 1;
int byte3 = 1;
int byte4 = 1;
long total = 1;
long mult2_24;
long mult2_16;
// end additional variables

void setup() 
{
  mult2_24 = 256 * 256 * 256;
  mult2_16 = 256 * 256;
  size(256, 256);  // Stage size
  noStroke();      // No border on the next thing drawn

  // Set the starting position of the ball (middle of the stage)
  xpos = width/2;
  ypos = height/2;

  // Print a list of the serial ports, for debugging purposes:
  println(Serial.list());

  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 115200);
}

void draw() {
  background(bgcolor);
  fill(fgcolor);
  // Draw the shape
  ellipse(xpos, ypos, 20, 20);
}

void serialEvent(Serial myPort) {
  // read a byte from the serial port:
  int inByte = myPort.read();
  // if this is the first byte received, and it's an A,
  // clear the serial buffer and note that you've
  // had first contact from the microcontroller. 
  // Otherwise, add the incoming byte to the array:
  if (firstContact == false) {
    if (inByte == 'A') { 
      myPort.clear();          // clear the serial port buffer
      firstContact = true;     // you've had first contact from the microcontroller
      myPort.write('A');       // ask for more
    } 
  } 
  else {
    // Add the latest byte from the serial port to array:
    serialInArray[serialCount] = inByte;
    serialCount++;

    // If we have 3 bytes:
    if (serialCount > 2 ) 
    {
      xpos = serialInArray[0];
      ypos = serialInArray[1];
      fgcolor = serialInArray[2];

      byte1 = serialInArray[0];
      byte2 = serialInArray[1];
      byte3 = serialInArray[2];
      //byte4 = serialInArray[3];
      //byte4 = serialInArray[3];
      //total = byte1 * 256 * 256 * 256 + byte2 * 256 * 256 + byte3 * 256;
      //total = byte1 * mult2_16 + byte2 * 256 + byte3;
      total = byte1 * 256;
      total = total + byte2;
      // print the values (for debugging purposes only):
      //println(xpos + "\t" + ypos + "\t" + fgcolor);
      println(byte1 + "\t" + byte2 + "\t" + byte3 + "\t" + total);

      // Send a capital A to request new sensor readings:
      myPort.write('A');
      // Reset serialCount:
      serialCount = 0;
    }
  }
}



