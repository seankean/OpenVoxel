// additional variables
int byte1 = 0;
int byte2 = 0;
int byte3 = 0;
int byte4 = 0;
//int freq = 1000;
//int testFreq = 1000;
//int freqDir = 1;
//long newOptoTime = 1;
//long oldOptoTime = 1;
unsigned long newSyncTime = 1;
unsigned long oldSyncTime = 1;
//long newSyncOutFreq = 1;
//long oldSyncOutFreq = 1;
//boolean syncOutPin = LOW;
//boolean newOpto = LOW;
//boolean oldOpto = HIGH;
boolean newSync = LOW;
boolean oldSync = LOW;
boolean newSer = LOW;
boolean oldSer = LOW;
//long optoTime = 1;
//long syncTime = 1;
//long optoSyncTime = 1;
boolean a = LOW;
//unsigned long div1 = 1;
//unsigned long div2 = 1;
//int sample = 0;
boolean newReadA = HIGH;
boolean newReadB = HIGH;
boolean newReadC = HIGH;
boolean newReadD = HIGH;
boolean oldReadA = HIGH;
boolean oldReadB = HIGH;
//long deltaFreq = 1;
//int incFreq = 1;
//int syncFreq = 0;
//boolean freqFlag = HIGH;
int serFlag = 0;      
//int syncTable[500];
int b = 1;
//int bb = 1;
//boolean ledFlag = HIGH;
//int delay1 = 1;
//int delayInc = 1;
//int feedbackFlag = 0;
//long newDelayTime = 0;
//long nextDelayTime = 0;
//long diffDelayTime = 0;
//long totalDelayTime = 30000;
//long diffSyncTime = 0;
//unsigned long syncDelayTime = 42000;
unsigned long syncDelayTime = 0;
//int constFreq = 1005;
//int freqLimit = 3500;
long newReadMicrosA = 0;
long oldReadMicrosA = 0;
long newReadMicrosB = 0;
long oldReadMicrosB = 0;
//int diffSyncTimeConst = 1;
//int ff = 0;     // flip flop
//int MotorDelay = 1;
int MotorPWM = 1;
long SYSCLOCK = 16000000;
int updown = 0;    // read computer parallel port on bit 2
//int updownOld = 0;
//int updownNew = 0;
//long updownSync = 0;
//long updownSync1 = 0;
//long updownSync2 = 0;
//long updownSync3 = 0;
int up = 1;
int down = 0;
//long updownNewMicros = 0;
//long updownOldMicros = 1;
//int updownOpto = 0;// end additional variables
char ser1 = '0';
//int ledReadNew = 0;
//int ledReadOld = 0;
//int ledWrite = 0;
//int ledCount = 0;
//int ledMax = 50;
//int ledOut = 0;
char sync = LOW;
int syncCount = 0;
//long syncDelay = 0;
long syncDelayMax = 50000;
long syncDelayInc = 1000;
int syncFlag = 1;
//int toggle = 0;
boolean hilo = LOW;    // state of sync input
int syncpin = 0;
int aa = 0;
int loopCount = 20;

void establishContact() 
{
  while (Serial.available() <= 0) 
  {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}

void enableIROut(int khz) {
  // Enables IR output.  The khz value controls the modulation frequency in kilohertz.
  // The IR output will be on pin 3 (OC2B).
  // This routine is designed for 36-40KHz; if you use it for other values, it's up to you
  // to make sure it gives reasonable results.  (Watch out for overflow / underflow / rounding.)
  // TIMER2 is used in phase-correct PWM mode, with OCR2A controlling the frequency and OCR2B
  // controlling the duty cycle.
  // There is no prescaling, so the output frequency is 16MHz / (2 * OCR2A)
  // To turn the output on and off, we leave the PWM running, but connect and disconnect the output pin.
  // A few hours staring at the ATmega documentation and this will all make sense.
  // See my Secrets of Arduino PWM at http://arcfn.com/2009/07/secrets-of-arduino-pwm.html for details.


  // Disable the Timer2 Interrupt (which is used for receiving IR)
  TIMSK2 &= ~_BV(TOIE2); //Timer2 Overflow Interrupt

  pinMode(11, OUTPUT);
  //digitalWrite(11, LOW); // When not sending PWM, we want it low

  // COM2A = 00: disconnect OC2A
  // COM2B = 00: disconnect OC2B; to send signal set to 10: OC2B non-inverted
  // WGM2 = 101: phase-correct PWM with OCRA as top
  // CS2 = 000: no prescaling
  TCCR2A = _BV(WGM20);
  TCCR2B = _BV(WGM22) | _BV(CS20);

  // The top value for the timer.  The modulation frequency will be SYSCLOCK / 2 / OCR2A.
  OCR2A = SYSCLOCK / 2 / khz / 1000;
  //OCR2B = OCR2A / 3; // 33% duty cycle
  OCR2B = OCR2A / 3; // 6% duty cycle
} 

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
    case 1: 
      mode = 0x01; 
      break;
    case 8: 
      mode = 0x02; 
      break;
    case 64: 
      mode = 0x03; 
      break;
    case 256: 
      mode = 0x04; 
      break;
    case 1024: 
      mode = 0x05; 
      break;
    default: 
      return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } 
    else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } 
  else if(pin == 3 || pin == 11) {
    switch(divisor) {
    case 1: 
      mode = 0x01; 
      break;
    case 8: 
      mode = 0x02; 
      break;
    case 32: 
      mode = 0x03; 
      break;
    case 64: 
      mode = 0x04; 
      break;
    case 128: 
      mode = 0x05; 
      break;
    case 256: 
      mode = 0x06; 
      break;
    case 1024: 
      mode = 0x7; 
      break;
    default: 
      return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}


void setPWMfreq1()
{
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);       // Output A freq
  TCCR2B = _BV(CS22);                                                 // Output B freq
  //OCR2A = 180;                                                        // Output A duty cycle
  OCR2B = 100;                
  OCR2A =255;                                                        // Output A duty cycle
  
  // Output B duty cycle
  // Output A frequency: 16 MHz / 64 / 256 = 976.5625Hz
  // Output A duty cycle: (180+1) / 256 = 70.7%
  // Output B frequency: 16 MHz / 64 / 256 = 976.5625Hz
  // Output B duty cycle: (50+1) / 256 = 19.9%
}


void setup()
{
  // start serial port at 9600 bps:
  Serial.begin(115200);
  pinMode(2, INPUT);   // computer sync input
  pinMode(3, INPUT);   // switch 3 (A)
  pinMode(4, INPUT);   // switch 4 (B)
  //pinMode(5, INPUT);   // switch
  //pinMode(6, INPUT);   // switch
  //pinMode(8, OUTPUT);  // Line cut. Brake motor ch B. Not connected
  pinMode(9, INPUT);   // Opto      Line cut to brake motor ch A 
  pinMode(10, OUTPUT);  // Motor direction output to Pololu High-Power Motor Driver 36v20 CS  
  pinMode(11, OUTPUT);   // Motor PWM to Pololu High-Power Motor Driver 36v20 CS        Old use:PWM motor ch B on Arduino motor shield
  //pinMode(12, OUTPUT);   // Not used   Direction motor ch A on Arduino motor shield
  pinMode(12, INPUT);   // v sync inout  photocell to detect video.f  Direction motor ch A on Arduino motor shield
  pinMode(13, OUTPUT);   // led + Direction motor ch B
  //analogWrite(3, 77);   // set PWM motor ch A to 77
  //analogWrite(11, 128);  // set PWM, motor ch B to 128;
  digitalWrite(12, 1);  // set DIR A, motor ch B to 1;
  digitalWrite(13, 1);  // set DIR B, motor ch A to 1;
  establishContact();  // send a byte to establish contact until receiver responds 
  byte1 = 0;
  byte2 = 0;
  byte3 = 0;
  //div1 = 256;
  //div1 = div1 * 256;
  //div1 = div1 * 256;
  //div2 = 256;
  //div2 = div2 * 256;
  //freq = 1000;
  //freq = 200;
  //constFreq = 551;    // this is the frequency to set the tone on pin 12. This frequency mtaches 8 frames being projected at 60 frames per second
  //freqLimit = 800;
  //setPwmFrequency(11, 8);
  //loadSyncTable();
  //tone(12,freq);
  //freqFlag = HIGH;
  serFlag = 0;
  //newOpto = digitalRead(9);
  //oldOpto = digitalRead(9);
  //delay1 = 0;
  //feedbackFlag = 0;
  //totalDelayTime = 30000;
  //syncDelayTime = 42000;
  //syncDelayTime = 0;
  //diffSyncTimeConst = 1;
  //analogWrite(3,250);
  //MotorDelay = 82;
  //analogWrite(11,20);
  //setPWMfreq1();
  //enableIROut(36000);    // sets PWM frequency to 36kHz
  setPwmFrequency(11,1);   // set pwm frequency to 31250
  //analogWrite(11,25);      // set pwm 
  //analogWrite(11,10);      // set pwm, height 1/2 inch for use with 28VDC supply 
  //analogWrite(11, 50);      // set pwm, height 1/2 inch for use with 28VDC supply 
  analogWrite(11, 35);      // set pwm, height 1/2 inch for use with 28VDC supply 
}



//  pinMode(6,OUTPUT);
//  TCCR0A = _BV(COM0A0) | _BV(WGM01); // toggle output set mode to ctc
//  TCCR0B = _BV(CS01) | _BV(CS00);  // CTC mode and prescaler to 64
//  OCR0A = 10;
//  OC1A corresponds to PB1, which is D9, and OC1B corresponds to PB2, which is D10.
//  OC2A and OC2B are not connected to Timer 1.




void loop()
{
  motorMove2();    
}

void motorMove2()
{
  syncpin = 2;
  sync = digitalRead(syncpin);
  showpin(2);
  hilo = LOW;
  if (Serial.available() > 0) 
  {
    ser1 = Serial.read();           // read an 'A' or 'B'
    Serial.write(ser1);
    Serial.print("Something\n");  
    //ser1 = '5';
    switch (ser1)
    {
      case '0':
        while (digitalRead(syncpin) != hilo) {}      
        updown = 1;
        Serial.write('0');          
      break;
      case '1':
        while (digitalRead(syncpin) != hilo) {}      
        updown = 1;
        Serial.write('1');          
      break;
      case '2':
        while (digitalRead(syncpin) != hilo) {}      
        updown = 1;
        Serial.write('2');          
      break;
      case '3':
        while (digitalRead(syncpin) != hilo) {}      
        updown = 1;
        oldSyncTime = micros();    // begin timing delay
        newSyncTime = oldSyncTime;
        syncFlag = 1;
        Serial.write('3');          
      break;
      case '4':
        while (digitalRead(syncpin) != hilo) {}      
        updown = 0;
        Serial.write('4');          
      break;
      case '5':
      while (digitalRead(syncpin) != hilo) {}      
        updown = 0;
        Serial.write('5');          
      break;
      case '6':
        while (digitalRead(syncpin) != hilo) {}      
        updown = 0;
        Serial.write('6');          
        /*
        Serial.write('S');        
        Serial.write('D');        
        Dec2Asc(syncDelayTime);        
        Serial.write(' ');
        Serial.write('N');
        Serial.write('S');
        Dec2Asc(newSyncTime);
        Serial.write(' ');
        Serial.write('L');
        Serial.write('S');
        Dec2Asc(oldSyncTime);
        Serial.write(' ');
        Dec2Asc(23456);
        Serial.write(' ');
        Dec2Asc(33333);
        Serial.write(' ');
        Dec2Asc(4000);
        Serial.write(' ');
        Dec2Asc(500);
        Serial.write(' ');
        Dec2Asc(60);
        Serial.write(' ');
        Dec2Asc(7);
        Serial.write(13);
        Serial.write(10);
        */

      break;
      case '7':
        while (digitalRead(syncpin) != hilo) {}      
        updown = 0;
        oldSyncTime = micros();    // begin timing delay
        newSyncTime = oldSyncTime;
        syncFlag = 1;
        Serial.write('7');        
      break;
      default: 
        Serial.write('z');          
      break;        
    }
    Serial.print("Something\n");
    delay(2);  // wait 2 milliseconds so v-sync falls
    readSwitchesDeBounce();
  }    
  newSyncTime = micros();    
  if ((1 == syncFlag) && ((newSyncTime - oldSyncTime) > syncDelayTime))   
  {
    digitalWrite(10, updown);
    syncFlag = 0;      // 
    //Serial.write('u');          

  }
  else 
  {
    //Serial.write('d');
  }    
}

void showpin(int pinnum)
{
  a = digitalRead(pinnum);
  digitalWrite(13, a);
}

void readSwitchesDeBounce()
{
  newReadA = digitalRead(3);
  newReadB = digitalRead(4);
  if ((LOW == newReadA) && (HIGH == oldReadA))
  {
    if (syncDelayTime < syncDelayMax)
    {
      syncDelayTime = syncDelayTime + syncDelayInc;
    }
    oldReadA = newReadA;    
  }
  if ((LOW == newReadB) && (HIGH == oldReadB))
  {
    if (syncDelayTime > (syncDelayInc + 1))      
    {
      syncDelayTime = syncDelayTime - syncDelayInc;
    }
    oldReadB = newReadB;
  }     
  loopCount++;
  if (loopCount >10)
  {
    oldReadA = newReadA;
    oldReadB = newReadB;
    loopCount = 1;
  }
}

void Dec2Asc(unsigned long n)
{
        Serial.write((48+(n % 1000000000)/100000000));
        Serial.write((48+(n % 100000000)/10000000));
        Serial.write((48+(n % 10000000)/1000000));
        Serial.write((48+(n % 1000000)/100000));
        Serial.write((48+(n % 100000)/10000));
        Serial.write((48+(n % 10000)/1000));
        Serial.write((48+(n % 1000)/100));
        Serial.write((48+(n % 100)/10));
        Serial.write((48+(n % 10)));
}


