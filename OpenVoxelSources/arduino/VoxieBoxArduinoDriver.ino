// additional variables
int byte1 = 0;
int byte2 = 0;
int byte3 = 0;
int byte4 = 0;

unsigned long newSyncTime = 1;
unsigned long oldSyncTime = 1;

boolean newSync = LOW;
boolean oldSync = LOW;
boolean newSer = LOW;
boolean oldSer = LOW;

boolean a = LOW;

int serFlag = 0;      

int b = 1;
unsigned long syncDelayTime = 0;

int MotorPWM = 1;
long SYSCLOCK = 16000000;

char ser1 = '0';
char sync = LOW;
int syncCount = 0;

int motorPin = 10;
int up = 0;
int down = 0;

int delayTime = 1;
int potDelayPin = 0;
int potPowerPin = 1;
unsigned long potDelay = 1; //used to sync frames
int minDelay = 1000;
int delayScaleFactor = 7;
int powerScaleFactor = 250;
int switchFrame = 3;
int moveDir = 0;
int power = 20;

boolean hilo = LOW;    // state of sync input
int syncpin = 0;
int aa = 0;
int loopCount = 20;
byte frameSelect = 0;
byte frameFromSerial = 0;

void establishContact() 
{
  while (Serial.available() <= 0) 
  {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}

void setup()
{
  // start serial port at 115200 bps:
  Serial.begin(115200);

  pinMode(potDelayPin, INPUT);
  pinMode(potPowerPin, INPUT);
  pinMode(2, INPUT);   // computer sync input
  pinMode(3, INPUT);   // switch 3 (A)
  pinMode(4, INPUT);   // switch 4 (B)
  pinMode(5, INPUT);   // switch 5 (C)  frameSelect +
  pinMode(6, INPUT);   // switch 6 (D)  frameSelect -
  pinMode(9, INPUT);   // Opto      Line cut to brake motor ch A 
  pinMode(10, OUTPUT);  // Motor direction output to Pololu High-Power Motor Driver 36v20 CS  
  pinMode(11, OUTPUT);   // Motor PWM to Pololu High-Power Motor Driver 36v20 CS        Old use:PWM motor ch B on Arduino motor shield
  pinMode(12, INPUT);   // v sync inout  photocell to detect video.f  Direction motor ch A on Arduino motor shield
  pinMode(13, OUTPUT);   // led + Direction motor ch B
  digitalWrite(12, 1);  // set DIR A, motor ch B to 1;
  digitalWrite(13, 1);  // set DIR B, motor ch A to 1;
  
  establishContact();  // send a byte to establish contact until receiver responds 
  
  byte1 = 0;
  byte2 = 0;
  byte3 = 0;
  
  serFlag = 0;
  
  setPwmFrequency(11,1);   // set pwm frequency to 31250
  analogWrite(11, power);      // set pwm, height 1/2 inch for use with 28VDC supply 
}

/** Our main loop **/
void loop()
{
  mainLoop();    
}

/** Delays the motor between frames **/
void motorDelay() {
  
  //read in data from the potentiometer
 int syncD = analogRead(potDelayPin);

 switchFrame = (syncD / 127) - 1; //the frame we're going to switch on
 potDelay = syncD % 127; //the delay between frame switches
 
 //scale the delay
 syncDelayTime = potDelay / delayScaleFactor;
 Serial.print("Sync delay: ");
 Serial.println(syncDelayTime, DEC);
// Serial.write(syncD);
 delay(syncDelayTime);
 
 
}


/** Sets the power of the motor**/
void setPower() {
  
  //read in data from the potentiometer
 int powerR = analogRead(potPowerPin);
 
 //scale the delay
 power = potDelay / powerScaleFactor;
 Serial.print("Power: ");
 Serial.println(power, DEC);
 
 
}

/** Handles the actual movement of the motor **/
void moveMotor(int frameNum){
  //pick a direction and move
  if(frameFromSerial == switchFrame)
  {
      digitalWrite(motorPin, 1); //move motor one way
  }
  else if(frameFromSerial == (switchFrame + 4) % 8)
  {
    digitalWrite(motorPin, 0); //move motor one way
  }
   
}



void mainLoop()
{

  syncpin = 2;
  sync = digitalRead(syncpin);
  
  showpin(2);
  hilo = LOW;
  int readFrame;
  
  if (Serial.available() > 0) 
  {
    
    char tmp = Serial.read(); //read a frame number
    switch(tmp){
      case '0':
         readFrame = 0;
         break;
      case '1':
         readFrame = 1;
         break;      
      case '2':
         readFrame = 2;
         break;
    case '3':
         readFrame = 3;
         break;
     case '4':
         readFrame = 4;
         break;
     case '5':
         readFrame = 5;
         break;
      case '6':
         readFrame = 6;
         break;         
      case '7':
         readFrame = 7;
         break;
      default:
         readFrame = 0;      
    }
    
    Serial.println(readFrame, DEC); //respond
     
    if(readFrame != frameFromSerial) {
       frameFromSerial = readFrame;
       Serial.print("Frame : ");
       Serial.println(readFrame, DEC);
       setPower();
	   motorDelay(); //delay the motor for sync
       moveMotor(readFrame); //move the motor  
    }
    
  }    
 
}
void showpin(int pinnum)
{
  a = digitalRead(pinnum);
  digitalWrite(13, a);
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


