/*
  Serial Call and Response
 Language: Wiring/Arduino
 
 This program sends an ASCII A (byte of value 65) on startup
 and repeats that until it gets some data in.
 Then it waits for a byte in the serial port, and 
 sends three sensor values whenever it gets a byte in.
 
 Thanks to Greg Shakar and Scott Fitzgerald for the improvements
 
   The circuit:
 * potentiometers attached to analog inputs 0 and 1 
 * pushbutton attached to digital I/O 2
 
 Created 26 Sept. 2005
 by Tom Igoe
 modified 30 Aug 2011
 by Tom Igoe and Scott Fitzgerald

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/SerialCallResponse

*/
// freq = 2680    for 3 images plus 120 milliseconds delay
// freq = 
int firstSensor = 0;    // first analog sensor
int secondSensor = 0;   // second analog sensor
int thirdSensor = 0;    // digital sensor
int inByte = 0;         // incoming serial byte

// additional variables
int byte1 = 0;
int byte2 = 0;
int byte3 = 0;
int byte4 = 0;
int freq = 1000;
int testFreq = 1000;
int freqDir = 1;
long newOptoTime = 1;
long oldOptoTime = 1;
long newSyncTime = 1;
long oldSyncTime = 1;
long newSyncOutFreq = 1;
long oldSyncOutFreq = 1;
boolean syncOutPin = LOW;
boolean newOpto = LOW;
boolean oldOpto = HIGH;
boolean newSync = LOW;
boolean oldSync = LOW;
boolean newSer = LOW;
boolean oldSer = LOW;
long optoTime = 1;
long syncTime = 1;
long optoSyncTime = 1;
boolean a = LOW;
unsigned long div1 = 1;
unsigned long div2 = 1;
int sample = 0;
boolean newReadA = HIGH;
boolean newReadB = HIGH;
boolean newReadC = HIGH;
boolean newReadD = HIGH;
boolean oldReadA = HIGH;
boolean oldReadB = HIGH;
long deltaFreq = 1;
int incFreq = 1;
//int syncFreq = 0;
boolean freqFlag = HIGH;
int serFlag = 0;      
int syncTable[500];
int b = 1;
//int bb = 1;
boolean ledFlag = HIGH;
int delay1 = 1;
int delayInc = 1;
int feedbackFlag = 0;
long newDelayTime = 0;
long nextDelayTime = 0;
long diffDelayTime = 0;
long totalDelayTime = 30000;
long diffSyncTime = 0;
long syncDelayTime = 42000;
int constFreq = 1005;
int freqLimit = 3500;
long newReadMicrosA = 0;
long oldReadMicrosA = 0;
long newReadMicrosB = 0;
long oldReadMicrosB = 0;
int diffSyncTimeConst = 1;
int ff = 0;     // flip flop 
// end additional variables


void setup()
{
  // start serial port at 9600 bps:
  Serial.begin(115200);
  //pinMode(2, INPUT);   // computer sync input
  pinMode(3, OUTPUT);   // switch
  //pinMode(4, INPUT);   // switch
  //pinMode(5, INPUT);   // switch
  //pinMode(6, INPUT);   // switch
  //pinMode(9, INPUT);   // opto
  pinMode(12, OUTPUT);   // oscillator out to motor driver
  pinMode(13, OUTPUT);   // led
  analogWrite(3, 77);   // set PWM motor ch A to 1, this is full on;
  digitalWrite(11, 1);  // set PWM, motor ch B to 128;
  digitalWrite(12, 1);  // set DIR A, motor ch B to 1;
  digitalWrite(13, 1);  // set DIR B, motor ch A to 1;
  establishContact();  // send a byte to establish contact until receiver responds 
  firstSensor = 1;
  secondSensor = 1;
  thirdSensor = 1;
  byte1 = 0;
  byte2 = 0;
  byte3 = 0;
  div1 = 256;
  div1 = div1 * 256;
  div1 = div1 * 256;
  div2 = 256;
  div2 = div2 * 256;
  freq = 1000;
  freq = 200;
  constFreq = 551;    // this is the frequency to set the tone on pin 12. This frequency mtaches 8 frames being projected at 60 frames per second
  freqLimit = 800;
  //setPwmFrequency(11, 8);
  loadSyncTable();
  //tone(12,freq);
  freqFlag = HIGH;
  serFlag = 0;
  //newOpto = digitalRead(9);
  //oldOpto = digitalRead(9);
  delay1 = 0;
  feedbackFlag = 0;
  totalDelayTime = 30000;
  syncDelayTime = 42000;
  diffSyncTimeConst = 1;
  //analogWrite(3,250);
}

void loop()
{
  motorMove2();    
}

void establishContact() 
{
  while (Serial.available() <= 0) 
  {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}

void motorMove2()
{
  digitalWrite(12,1);        // dir on motor is 1
  delay(82);
  if (Serial.available() > 0) 
    {
      Serial.write('B');
      Serial.write(' ');
      Serial.write('B');
      Serial.write(' ');
      
      Serial.write('T');
      Serial.write(' ');
      Serial.write('T');
      Serial.write(' ');
      /*
      Serial.write(byte1);
      Serial.write(byte2);
      Serial.write(byte3);
      */
    }
  digitalWrite(12,0);      // dir on motor is 0
  delay(82);
  ff = ff ^ 1;    
  byte1 = syncDelayTime >> 8;
  byte2 = syncDelayTime & 255;
  byte3 = ff;
}



void motorMove()
{
  readSwitchesDeBounce();
  newSync = digitalRead(2);
  if (newSync != oldSync)
  {
    newSyncTime = micros();
    if (newSyncTime > oldSyncTime)
    {
      syncTime = newSyncTime - oldSyncTime;   // syncTime is always positive        
    }
  }
  oldSync = newSync;
  oldSyncTime = newSyncTime;

  newOpto = digitalRead(9);
  //digitalWrite(13,newOpto);
  if ((LOW == newOpto) && (HIGH == oldOpto))
  {
    newOptoTime = micros();
    if (Serial.available() > 0) 
    {
      inByte = Serial.read();
      Serial.write(byte1);
      Serial.write(byte2);
      Serial.write(byte3);
    }
    if (newOptoTime > oldOptoTime)
    {
      optoTime = newOptoTime - oldOptoTime;    // optoTime is always positive    
    }
    if (newOptoTime > newSyncTime)
    {
      optoSyncTime = newOptoTime - newSyncTime;  // optoSyncTime is the time between computer sync pulse and falling edge of opto. It is always positive
    }
        
    //bb =  syncTime >> 12;       // not used
    //syncFreq =  syncTable[bb];  // not used
    if (1 != feedbackFlag)      // if feedback is off, inc freq untilit reaches 2680
    {    
      newReadD = digitalRead(6);
      if (HIGH == newReadD)
      {
        if (freq < (constFreq - 50))
        {
          freq = freq + 50; 
        }
        else
        {
          //if (freq < 2680)
          if (freq < constFreq)
          {
            freq = freq + 1; 
          }
          else
          {
            feedbackFlag = 1;
          }
        }
      }
    }
    
    diffSyncTimeConst = 2;
    if (1 == feedbackFlag) 
    {
      if(HIGH == newReadC)   // proceed to synchronize if switch 5 is not pressed
      { 
        if (((syncTime / 2) > (optoSyncTime - syncDelayTime)) && (0 < (optoSyncTime - syncDelayTime)))
        {
          diffSyncTime = syncTime - (optoSyncTime - syncDelayTime);
          //diffSyncTime = (diffSyncTime >> 11);
          if (diffSyncTime < 1)
          {
            diffSyncTime = 0;
          }
          //if (diffSyncTime > 1)
          if (diffSyncTime > diffSyncTimeConst)
          {
            //diffSyncTime = 1;
            diffSyncTime = diffSyncTimeConst;
          }
          //if (freq < ((2680 + 10)) && (1 == feedbackFlag))
          //if ((freq < (constFreq + 1)) && (1 == feedbackFlag))
          //if ((freq < (constFreq + diffSyncTimeConst)) && (1 == feedbackFlag))
          if (freq < (constFreq + diffSyncTimeConst))   // as long as freq is within bounds, increment it
          {
            //freq = freq + 1;
            freq = freq + diffSyncTime;
          }
        }
        else
        {
          diffSyncTime = syncTime - (optoSyncTime - syncDelayTime);
          //diffSyncTime = (diffSyncTime >> 11);
          if (diffSyncTime < 1)
          {
            diffSyncTime = 0;
          }
          //if (diffSyncTime > 1)
          if (diffSyncTime > diffSyncTimeConst)
          {
            //diffSyncTime = 1;
            diffSyncTime = diffSyncTimeConst;
          }
          //if ((freq > (2680 - 10)) && (1 == feedbackFlag))
          //if ((freq > (constFreq - 1)) && (1 == feedbackFlag))
          //if ((freq > (constFreq - diffSyncTimeConst)) && (1 == feedbackFlag))
          //if ((freq > (constFreq)) && (1 == feedbackFlag))
          //if (freq > (constFreq)
          if (freq > (constFreq - diffSyncTimeConst))   // as long as freq is within bounds, decrement it
          {
            //freq = freq - 1;
            //freq = freq - diffSyncTime;
            freq = freq - diffSyncTime;
          }
        }
      }
      else         // manually increase frequency of motor
      {
        freq = constFreq + 4;
      }
    }    
    //byte1 = optoTime >> 16;
    //byte2 = optoTime >> 8;
    //byte3 = optoTime & 255;

    //byte1 = syncTime >> 16;
    //byte2 = syncTime >> 8;
    //byte3 = syncTime & 255;

    //byte1 = optoSyncTime >> 8;
    //byte2 = optoSyncTime >> 255;

    byte1 = syncDelayTime >> 8;
    byte2 = syncDelayTime & 255;
    byte3 = freq - 400;
    //byte3 = constFreq - 400;
    //byte3 = delay1;
  }
  oldOpto = newOpto;
  oldOptoTime = newOptoTime;
}
  

void readSwitchesDeBounce()
{
  newReadA = digitalRead(3);
  newReadB = digitalRead(4);
  if ((LOW == newReadA) && (HIGH == oldReadA))
  {
    newReadMicrosA = micros();
    if (200000 < (newReadMicrosA - oldReadMicrosA)) 
    {
      oldReadMicrosA = newReadMicrosA;
      if (freqLimit > constFreq)
      {
        constFreq = constFreq + 1;
      }
    }
  }
  if ((LOW == newReadB) && (HIGH == oldReadB))
  {
    newReadMicrosB = micros();
    if (200000 < (newReadMicrosB - oldReadMicrosB))
    {
      oldReadMicrosB = newReadMicrosB;
      showpin(4);
      if (1 < constFreq)
      {
        if (freqLimit > constFreq)
        {
          constFreq = constFreq - 1;
        }
      }
    }
  }     
  oldReadA = newReadA;
  oldReadB = newReadB;
}






void showpin(int pinnum)
{
  a = digitalRead(pinnum);
  digitalWrite(13, a);
}

void ledblink()
{
  if (LOW == ledFlag)
  {
    ledFlag = HIGH;
    digitalWrite(13,LOW);
  }
  else
  {
    ledFlag = LOW;
    digitalWrite(13,HIGH);
  }
}

void readSwitchesAB()
{
      newReadA = digitalRead(3);
      newReadB = digitalRead(4);
      if ((LOW == newReadA) && (HIGH == oldReadA))
      {
        feedbackFlag = 0;     // turn off frequency feedback
        if (2750 > freq)
        {
          freq = freq + incFreq;
        }
      }
      if ((LOW == newReadB) && (HIGH == oldReadB))
      {
        feedbackFlag = 0;     // turn off frequency feedback
        if (1 < freq)
        {
          freq = freq - incFreq;
        }
      }     
}


void readSwitches()
{
  newReadA = digitalRead(3);
  newReadB = digitalRead(4);
  newReadC = digitalRead(5);
  newReadD = digitalRead(6);
  if (HIGH == newReadD)          // if 6 is pressed, change freq by 10
  {                              // else change by 1
    incFreq = 10;
  }
  else
  {
    incFreq = 1;
  }
  if ((LOW == newReadA) && (HIGH == oldReadA))  // switch 3 increments freq
  {
    if (freqLimit > freq)
    //if (2740 > freq)
    {
      constFreq = constFreq + 1;
      //freq = freq + incFreq;
    }
  }
  if ((LOW == newReadB) && (HIGH == oldReadB))  // switch 4 decrements freq
  {
    if (1 < freq)
    {
      constFreq = constFreq - 1;
      //freq = freq - incFreq;
    }
  }     
  oldReadA = newReadA;
  oldReadB = newReadB;
}  


void varyDelay()
{
  newReadA = digitalRead(3);
  newReadB = digitalRead(4);
  newReadC = digitalRead(5);
  if (HIGH == newReadC)
  {
    delayInc = 2000;
  }
  else
  {
    delayInc = 100;
  }
  if ((LOW == newReadA) && (HIGH == oldReadA))
  {
    newReadMicrosA = micros();
    if (200000 < (newReadMicrosA - oldReadMicrosA)) 
    {
      oldReadMicrosA = newReadMicrosA;
      if (300000 > syncDelayTime)
      {
        syncDelayTime = syncDelayTime + delayInc;
      }
    }
  }
  if ((LOW == newReadB) && (HIGH == oldReadB))
  {
    newReadMicrosB = micros();
    if (200000 < (newReadMicrosB - oldReadMicrosB))
    {
      oldReadMicrosB = newReadMicrosB;
      showpin(4);
      if (1 < syncDelayTime)
      {
        syncDelayTime = syncDelayTime - delayInc;
      }
    }
  }     
  oldReadA = newReadA;
  oldReadB = newReadB;
}


void loop_test()
{
  delay(200);
  digitalWrite(10,LOW);
  delay(200);
  digitalWrite(10,HIGH);
}

/*
  if ((newSyncOutFreq - oldSyncOutFreq) > 100000)
  {
    if (LOW == syncOutPin)
    {
      digitalWrite(10, HIGH);
      syncOutPin = HIGH;
    }
    else
    {
      digitalWrite(10, LOW);
      syncOutPin = LOW;
    }
    oldSyncOutFreq = newSyncOutFreq;
  }
  */  


void loadSyncTable()
{
syncTable[1] = 1001;
syncTable[2] = 1002;
syncTable[3] = 1003;
syncTable[4] = 1004;
syncTable[5] = 1005;
syncTable[6] = 1006;
syncTable[7] = 1007;
syncTable[8] = 1008;
syncTable[9] = 1009;
syncTable[10] = 1010;
syncTable[11] = 1011;
syncTable[12] = 1012;
syncTable[13] = 1013;
syncTable[14] = 1014;
syncTable[15] = 1015;
syncTable[16] = 1016;
syncTable[17] = 1017;
syncTable[18] = 1018;
syncTable[19] = 1019;
syncTable[20] = 1020;
syncTable[21] = 1021;
syncTable[22] = 1022;
syncTable[23] = 1023;
syncTable[24] = 1024;
syncTable[25] = 1025;
syncTable[26] = 1026;
syncTable[27] = 1027;
syncTable[28] = 1028;
syncTable[29] = 1029;
syncTable[30] = 1030;
syncTable[31] = 1031;
syncTable[32] = 1032;
syncTable[33] = 1033;
syncTable[34] = 1034;
syncTable[35] = 1035;
syncTable[36] = 1036;
syncTable[37] = 1037;
syncTable[38] = 1038;
syncTable[39] = 1039;
syncTable[40] = 1040;
syncTable[41] = 1041;
syncTable[42] = 1042;
syncTable[43] = 1043;
syncTable[44] = 1044;
syncTable[45] = 1045;
syncTable[46] = 1046;
syncTable[47] = 1047;
syncTable[48] = 1048;
syncTable[49] = 1049;
syncTable[50] = 1050;
syncTable[51] = 1051;
syncTable[52] = 1052;
syncTable[53] = 1053;
syncTable[54] = 1054;
syncTable[55] = 1055;
syncTable[56] = 1056;
syncTable[57] = 1057;
syncTable[58] = 1058;
syncTable[59] = 1059;
syncTable[60] = 1060;
syncTable[61] = 1061;
syncTable[62] = 1062;
syncTable[63] = 1063;
syncTable[64] = 1064;
syncTable[65] = 1065;
syncTable[66] = 1066;
syncTable[67] = 1067;
syncTable[68] = 1068;
syncTable[69] = 1069;
syncTable[70] = 1070;
syncTable[71] = 1071;
syncTable[72] = 1072;
syncTable[73] = 1073;
syncTable[74] = 1074;
syncTable[75] = 1075;
syncTable[76] = 1076;
syncTable[77] = 1077;
syncTable[78] = 1078;
syncTable[79] = 1079;
syncTable[80] = 1080;
syncTable[81] = 1081;
syncTable[82] = 1082;
syncTable[83] = 1083;
syncTable[84] = 1084;
syncTable[85] = 1085;
syncTable[86] = 1086;
syncTable[87] = 1087;
syncTable[88] = 1088;
syncTable[89] = 1089;
syncTable[90] = 1090;
syncTable[91] = 1091;
syncTable[92] = 1092;
syncTable[93] = 1093;
syncTable[94] = 1094;
syncTable[95] = 1095;
syncTable[96] = 1096;
syncTable[97] = 1097;
syncTable[98] = 1098;
syncTable[99] = 1099;
syncTable[100] = 1100;
syncTable[101] = 1101;
syncTable[102] = 1102;
syncTable[103] = 1103;
syncTable[104] = 1104;
syncTable[105] = 1105;
syncTable[106] = 1106;
syncTable[107] = 1107;
syncTable[108] = 1108;
syncTable[109] = 1109;
syncTable[110] = 1110;
syncTable[111] = 1111;
syncTable[112] = 1112;
syncTable[113] = 1113;
syncTable[114] = 1114;
syncTable[115] = 1115;
syncTable[116] = 1116;
syncTable[117] = 1117;
syncTable[118] = 1118;
syncTable[119] = 1119;
syncTable[120] = 1120;
syncTable[121] = 1121;
syncTable[122] = 1122;
syncTable[123] = 1123;
syncTable[124] = 1124;
syncTable[125] = 1125;
syncTable[126] = 1126;
syncTable[127] = 1127;
syncTable[128] = 1128;
syncTable[129] = 1129;
syncTable[130] = 1130;
syncTable[131] = 1131;
syncTable[132] = 1132;
syncTable[133] = 1133;
syncTable[134] = 1134;
syncTable[135] = 1135;
syncTable[136] = 1136;
syncTable[137] = 1137;
syncTable[138] = 1138;
syncTable[139] = 1139;
syncTable[140] = 1140;
syncTable[141] = 1141;
syncTable[142] = 1142;
syncTable[143] = 1143;
syncTable[144] = 1144;
syncTable[145] = 1145;
syncTable[146] = 1146;
syncTable[147] = 1147;
syncTable[148] = 1148;
syncTable[149] = 1149;
syncTable[150] = 1150;
syncTable[151] = 1151;
syncTable[152] = 1152;
syncTable[153] = 1153;
syncTable[154] = 1154;
syncTable[155] = 1155;
syncTable[156] = 1156;
syncTable[157] = 1157;
syncTable[158] = 1158;
syncTable[159] = 1159;
syncTable[160] = 1160;
syncTable[161] = 1161;
syncTable[162] = 1162;
syncTable[163] = 1163;
syncTable[164] = 1164;
syncTable[165] = 1165;
syncTable[166] = 1166;
syncTable[167] = 1167;
syncTable[168] = 1168;
syncTable[169] = 1169;
syncTable[170] = 1170;
syncTable[171] = 1171;
syncTable[172] = 1172;


syncTable[168] = 2927;
syncTable[169] = 2902;
syncTable[170] = 2877;
syncTable[171] = 2852;

syncTable[172] = 2827;
syncTable[173] = 2802;
syncTable[174] = 2760;
syncTable[175] = 2738;
syncTable[176] = 2717;

syncTable[177] = 2700;
syncTable[178] = 2683;
syncTable[179] = 2665;
syncTable[180] = 2648;
syncTable[181] = 2630;
syncTable[182] = 2613;
syncTable[183] = 2597;
syncTable[184] = 2584;
syncTable[185] = 2572;
syncTable[186] = 2559;
syncTable[187] = 2547;
syncTable[188] = 2534;
syncTable[189] = 2522;
syncTable[190] = 2509;
syncTable[191] = 2497;
syncTable[192] = 2483;
syncTable[193] = 2470;
syncTable[194] = 2457;
syncTable[195] = 2443;
syncTable[196] = 2430;
syncTable[197] = 2417;
syncTable[198] = 2403;
syncTable[199] = 2392;
syncTable[200] = 2382;
syncTable[201] = 2372;
syncTable[202] = 2362;
syncTable[203] = 2351;
syncTable[204] = 2341;
syncTable[205] = 2331;
syncTable[206] = 2321;
syncTable[207] = 2310;
syncTable[208] = 2300;
syncTable[209] = 2289;
syncTable[210] = 2279;
syncTable[211] = 2268;
syncTable[212] = 2258;
syncTable[213] = 2247;
syncTable[214] = 2237;
syncTable[215] = 2226;
syncTable[216] = 2216;
syncTable[217] = 2205;
syncTable[218] = 2195;
syncTable[219] = 2185;
syncTable[220] = 2174;
syncTable[221] = 2164;
syncTable[222] = 2154;
syncTable[223] = 2144;
syncTable[224] = 2133;
syncTable[225] = 2123;
syncTable[226] = 2113;
syncTable[227] = 2103;
syncTable[228] = 2093;
syncTable[229] = 2085;
syncTable[230] = 2076;
syncTable[231] = 2067;
syncTable[232] = 2059;
syncTable[233] = 2050;
syncTable[234] = 2041;
syncTable[235] = 2033;
syncTable[236] = 2024;
syncTable[237] = 2015;
syncTable[238] = 2007;
syncTable[239] = 1998;
syncTable[240] = 1991;
syncTable[241] = 1983;
syncTable[242] = 1976;
syncTable[243] = 1969;
syncTable[244] = 1961;
syncTable[245] = 1954;
syncTable[246] = 1946;
syncTable[247] = 1939;
syncTable[248] = 1931;
syncTable[249] = 1924;
syncTable[250] = 1917;
syncTable[251] = 1909;
syncTable[252] = 1902;
syncTable[253] = 1894;
syncTable[254] = 1887;
syncTable[255] = 1880;
syncTable[256] = 1872;
syncTable[257] = 1865;
syncTable[258] = 1857;
syncTable[259] = 1850;
syncTable[260] = 1843;
syncTable[261] = 1835;
syncTable[262] = 1828;
syncTable[263] = 1820;
syncTable[264] = 1813;
syncTable[265] = 1806;
syncTable[266] = 1798;
syncTable[267] = 1792;
syncTable[268] = 1785;
syncTable[269] = 1779;
syncTable[270] = 1773;
syncTable[271] = 1766;
syncTable[272] = 1760;
syncTable[273] = 1753;
syncTable[274] = 1747;
syncTable[275] = 1740;
syncTable[276] = 1734;
syncTable[277] = 1727;
syncTable[278] = 1721;
syncTable[279] = 1715;
syncTable[280] = 1708;
syncTable[281] = 1702;
syncTable[282] = 1696;
syncTable[283] = 1690;
syncTable[284] = 1684;
syncTable[285] = 1679;
syncTable[286] = 1673;
syncTable[287] = 1667;
syncTable[288] = 1661;
syncTable[289] = 1656;
syncTable[290] = 1650;
syncTable[291] = 1644;
syncTable[292] = 1639;
syncTable[293] = 1633;
syncTable[294] = 1627;
syncTable[295] = 1621;
syncTable[296] = 1616;
syncTable[297] = 1610;
syncTable[298] = 1604;
syncTable[299] = 1599;
syncTable[300] = 1594;
syncTable[301] = 1589;
syncTable[302] = 1585;
syncTable[303] = 1580;
syncTable[304] = 1575;
syncTable[305] = 1570;
syncTable[306] = 1565;
syncTable[307] = 1561;
syncTable[308] = 1556;
syncTable[309] = 1551;
syncTable[310] = 1546;
syncTable[311] = 1542;
syncTable[312] = 1537;
syncTable[313] = 1532;
syncTable[314] = 1527;
syncTable[315] = 1523;
syncTable[316] = 1518;
syncTable[317] = 1513;
syncTable[318] = 1508;
syncTable[319] = 1504;
syncTable[320] = 1499;
syncTable[321] = 1494;
syncTable[322] = 1490;
syncTable[323] = 1486;
syncTable[324] = 1481;
syncTable[325] = 1477;
syncTable[326] = 1472;
syncTable[327] = 1468;
syncTable[328] = 1463;
syncTable[329] = 1459;
syncTable[330] = 1454;
syncTable[331] = 1450;
syncTable[332] = 1446;
syncTable[333] = 1441;
syncTable[334] = 1437;
syncTable[335] = 1432;
syncTable[336] = 1428;
syncTable[337] = 1423;
syncTable[338] = 1419;
syncTable[339] = 1414;
syncTable[340] = 1410;
syncTable[341] = 1406;
syncTable[342] = 1401;
syncTable[343] = 1397;
syncTable[344] = 1393;
syncTable[345] = 1389;
syncTable[346] = 1385;
syncTable[347] = 1382;
syncTable[348] = 1378;
syncTable[349] = 1374;
syncTable[350] = 1370;
syncTable[351] = 1366;
syncTable[352] = 1362;
syncTable[353] = 1358;
syncTable[354] = 1354;
syncTable[355] = 1350;
syncTable[356] = 1347;
syncTable[357] = 1343;
syncTable[358] = 1339;
syncTable[359] = 1335;
syncTable[360] = 1331;
syncTable[361] = 1327;
syncTable[362] = 1323;
syncTable[363] = 1319;
syncTable[364] = 1316;
syncTable[365] = 1312;
syncTable[366] = 1308;
syncTable[367] = 1304;
syncTable[368] = 1300;
syncTable[369] = 1297;
syncTable[370] = 1293;
syncTable[371] = 1290;
syncTable[372] = 1287;
syncTable[373] = 1284;
syncTable[374] = 1280;
syncTable[375] = 1277;
syncTable[376] = 1274;
syncTable[377] = 1271;
syncTable[378] = 1267;
syncTable[379] = 1264;
syncTable[380] = 1261;
syncTable[381] = 1258;
syncTable[382] = 1254;
syncTable[383] = 1251;
syncTable[384] = 1248;
syncTable[385] = 1245;
syncTable[386] = 1241;
syncTable[387] = 1238;
syncTable[388] = 1235;
syncTable[389] = 1232;
syncTable[390] = 1228;
syncTable[391] = 1225;
syncTable[392] = 1222;
syncTable[393] = 1219;
syncTable[394] = 1215;
syncTable[395] = 1212;
syncTable[396] = 1209;
syncTable[397] = 1206;
syncTable[398] = 1202;
syncTable[399] = 1199;
syncTable[400] = 1197;
syncTable[401] = 1194;
syncTable[402] = 1191;
syncTable[403] = 1188;
syncTable[404] = 1186;
syncTable[405] = 1183;
syncTable[406] = 1180;
syncTable[407] = 1178;
syncTable[408] = 1175;
syncTable[409] = 1172;
syncTable[410] = 1169;
syncTable[411] = 1167;
syncTable[412] = 1164;
syncTable[413] = 1161;
syncTable[414] = 1159;
syncTable[415] = 1156;
syncTable[416] = 1153;
syncTable[417] = 1150;
syncTable[418] = 1148;
syncTable[419] = 1145;
syncTable[420] = 1142;
syncTable[421] = 1139;
syncTable[422] = 1137;
syncTable[423] = 1134;
syncTable[424] = 1131;
syncTable[425] = 1129;
syncTable[426] = 1126;
syncTable[427] = 1123;
syncTable[428] = 1120;
syncTable[429] = 1118;
syncTable[430] = 1115;
syncTable[431] = 1112;
syncTable[432] = 1110;
syncTable[433] = 1107;
syncTable[434] = 1104;
syncTable[435] = 1101;
syncTable[436] = 1099;
syncTable[437] = 1096;
syncTable[438] = 1094;
syncTable[439] = 1092;
syncTable[440] = 1089;
syncTable[441] = 1087;
syncTable[442] = 1085;
syncTable[443] = 1082;
syncTable[444] = 1080;
syncTable[445] = 1078;
syncTable[446] = 1075;
syncTable[447] = 1073;
syncTable[448] = 1070;
syncTable[449] = 1068;
syncTable[450] = 1066;
syncTable[451] = 1063;
syncTable[452] = 1061;
syncTable[453] = 1059;
syncTable[454] = 1056;
syncTable[455] = 1054;
syncTable[456] = 1051;
syncTable[457] = 1049;
syncTable[458] = 1047;
syncTable[459] = 1044;
syncTable[460] = 1042;
syncTable[461] = 1040;
syncTable[462] = 1037;
syncTable[463] = 1035;
syncTable[464] = 1033;
syncTable[465] = 1030;
syncTable[466] = 1028;
syncTable[467] = 1025;
syncTable[468] = 1023;
syncTable[469] = 1021;
syncTable[470] = 1018;
syncTable[471] = 1016;
syncTable[472] = 1014;
syncTable[473] = 1011;
syncTable[474] = 1009;
syncTable[475] = 1007;
syncTable[476] = 1004;
syncTable[477] = 1002;
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






    /*
    optoTime = 111;
    optoTime = optoTime << 8;
    optoTime = optoTime + 22;
    optoTime = optoTime << 8;
    optoTime = optoTime + 43;
    optoTime = optoTime << 8;
    */
 
  
/*
Processing sketch to run with this example:

// This example code is in the public domain.

import processing.serial.*;

int bgcolor;                 // Background color
int fgcolor;                 // Fill color
Serial myPort;                       // The serial port
int[] serialInArray = new int[3];    // Where we'll put what we receive
int serialCount = 0;                 // A count of how many bytes we receive
int xpos, ypos;                  // Starting position of the ball
boolean firstContact = false;        // Whether we've heard from the microcontroller

void setup() {
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
  myPort = new Serial(this, portName, 9600);
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
    if (serialCount > 2 ) {
      xpos = serialInArray[0];
      ypos = serialInArray[1];
      fgcolor = serialInArray[2];

      // print the values (for debugging purposes only):
      println(xpos + "\t" + ypos + "\t" + fgcolor);

      // Send a capital A to request new sensor readings:
      myPort.write('A');
      // Reset serialCount:
      serialCount = 0;
    }
  }
}
*/

/*
Max/MSP version 5 patch to run with this example:

----------begin_max5_patcher----------
2569.3oc2as0jiZqD9YO+Jzw09PRc75BIAX671TaUop8gy4gLoNmG1YqsjAY
rxhAGPLW1T4+dZIAd.aCFeiEuYqXFABQqu9qa0Rp0ec2fgyiegmND8KnOgFL
3utav.8sT2XPd4ACWwdwKjkpq1vU7zTV.e3Hyyj7Wj5665Tbq3LYHWJecM2z
tCGh9b9iVyjdKEQAeIg6IMOkRmM1ZDx10UcgRF6LBgmN1Zy6H70se77+38yJ
9DKhijQrU5Ovv6SDrvhmDksRDAedsvRJU8Tw2zUGSfuyl5ZjUckwpa922cm5
mQsDLh3OCx0NXQJODgqENlyhBFNpkvBchFVzfCwZ+vh60DVHm.r3EuZEORtC
t7.WISnOvBCe+uwSWGGkxQnGidL5AdjeJhgl+pjifuNRtjiRMUecbhbDhE4i
R3LnVTcsRQhnwHzCfXhVDmvChyfZ3EGFmLB8x53Tyq7J7Wn3EPS6IR7B4nrT
.n0M+SrvLnYR3xrjHtOZQR7ps+tiMh2+MVx+EzuuTjhz5JDzSy.KAn5Lir5y
eR3AhdjtTL7SBB5SpO8VMIBZjfXsPDC2GpCCojIP1L89EFIC45f9o6e3Ce7i
n6+YUCmJYIxr0iA4.ZvuxUxwyLgo+ajDUCLR8AizsLfnQn7l.8LbW9SfXIjv
qAZdzJ.1P9LIartS5AvqDvArM590I.ayZ1iQyeE8fWrTh9Ug7aA7DVnuFW+c
.q9XP7F+.ghHtGnBzJZLtdhsskshK6PLV85BXmZL3cNRlM9XX1VWPlsLQD.n
C5m.Mwmje9mUpDOE4RDrT99P9BIPMidBdUAP5AV08ggFdSB6YEWPgoqShg2Q
yOeV.OeIa8ZPSNmq32n+C6Efq9m.kETcfimb96Xz+WotkJtYgTrPjvA9Onn2
gE.bNV5WQ2m3mIhh0LmRs0d0lz5UlDiWJGKGs1jXtTixz8lQalvEQBIHVvGM
UqlBXJONOqQZi2BvfjosuWrWPiTOngmXo8oatfoZPiZWCnYeq.ZdK4desvWD
GXYdBQtmLvk1iCu+wgJ12bdfHBLF.QNyioLGTVCKjJGSFPW8vUYQBySUtKWw
70t0f+bdXr2WQoKy.i.+3miNZJqsqA8czvNgRajxR6aneMQbrF.XkqDMzaFo
6wgmV.YDrNjCWaC.4psvwypAfH6Ef9e7DeVDauPDcePjUcAkUVN4I4.SNx.s
gHTMjVJvSJU6ACeq23nGfYlsoKYYT1khiBv6.Ekhq6SVE2zmu3XZiXvO8a0W
WiJ+Tslhn0f+YvFRSv296xxBkeY+fS0muf4wq8kqQULXXPhvONRIFUdW0sK9
f.Gvn6cJK45ZDwVumWVFGGNmk7jHULOjWQS.rYVjXE39TJLRDDWQwCEqVmHL
VratGOhAswxTuj3vvJMk4IOsmmXB95YgubotsdCupL8lRLmJ1YUteiS2opQ2
hjf4.H4T7+kqT81b0Fw+DGSrPZRyro5Bk7Kssom8jxeuZ8OUa3+6ZDhG6LyA
OcR0Wb6oHMnvok4OFcs.VK0+NOHkjCoF5ryrCBot2zPZkwF1cFoJVZy.ZwLS
2YFp0xYsLwvXtXlBOA2..6TK.ukep5FYsgQW2C5R6FzcMChIw5RvXMF+4DV7
TqCBnzSFPsOE.sinq+afR0HPpG03PV+UHm1GFKImLVR9QGKycj1ZnDe6BkMM
vDDVMKYDZMCvrXXtMn2gQuifdGE8N6KhgewExAGpx5ldnJs7b1rRmIpUKNmN
taHqauXRSqETZfYU5IEy7U0fC6cfAlT137vnwrenQCp0QgFtV8Tzv74FdfQ5
HSGSg+y1dj9uaWWF2pXs1ZIKNht7aScTs1L0LKLcuQ878iEowYIdE58h.dPU
6S97ToHZybo+zaNH2phKE99Um4pFtE9qiAJUt.h9bqzdGsb6zV41s+I231H2
S5WxMts3shPQ5OxM4XjaZuQtUCt1d415FTtw8K4d1wf23aP4lzqvaWq1J2N8
K+fsUtc6W768LL3sgbO46gbmeSnCX1tjT1Sb+u.eFHDwuvjxDw7LoIDrxaex
4uaBM9vCsYFAgwyYg4asylVoRauiTscac2aHwkYmzrpcWyJOsi8NkCb995N8
sLYptT1wYxMRpL8udeCYxzAQjolDBf51BDw4FAQToB.LfJ9DS2MCjju8ylcV
rVHwtuAIx3ffP9YyGLoKhY8JpsySabC1u1pWqSS8hM6RrcqTuV2PoyXCo2Y6
xmwbduYKMroMAL1S6aIzXnmesc+PQpT08KtpLBF0xbrXV9pz3t4x9vC5rivT
v9xo2kpTPLrQq8Qsydvwjze1js23fJcSmiNWRveuxj0mXga7OsuEl1jTWtlt
sIGdqqaiut85SJIixVMmmbHEu1tuIkus6jRnfiaiJ+aJcOoAcusILPWyfbGP
2Os+o7anaianaSlRZc2lX8CKmmZWFFZlySH8OR+EBFJFfKGFbZDF5g190LhX
Vzao5wgvnRWZAR4XxF37zsrVnZ10EpnWNn5agnfj3r0HZ8QR2xnGrMAMNA23
.HG+3njuSrHHdZnKBbnCeFgZWr0XSbU4YgEooXqoVWyLZldIym7PAXpsjmvU
oMtWXbJe6iRSCCGQMo4MYlgzX03Anh3dyjj8U.EUh3dLXxz7T51oMXxj9FlT
2IOTSMNwUiI2xwvRn6jfnU.Dbea550AH5SYF6TONl1k3H13lPDbu67XVmYyG
pX1DvA3Aolut5joTx1Isov5yWzJCIgXMoQim9lsyYtvcDhwzHOPNRwu6kUf+
9rvc+4JtLI9sjcrlAUaQ2rXfTmlTwXxMi6.8Yr3z7FjuBlFRuYY7q0a.8lY4
L0F7LzLWKqyZ0sx4KTrloLswU6EeUOHeWx02323L+Buhhn0YRz7rEKTmm4m3
IuBFXnUhPv6I2KNxO8nO8iTy4IKeo.sZ5vOhuYNwnlAXTGna0gztokIwrj.X
WCLfabXDbmECl9qWMO8Lvw16+cNnry9dWIsNpYKuUl.kpzNa2892p6czPsUj
bnsPlbONQhByHUkxwTr5B0d5lRmov51BYcVmBeTbKDIpS2JSUxFwZjIxrtWl
tzTehEUwrbLqlH1rP5UKkmgyDplCpKctFLSZQOYKqpCawfmYRR+7oXYuoz4h
6VsQZmzstbZCWvw9z74XN+h1NlSrdkRTmxnqtTW37zoas9IsxgNoakIRakIb
24QpshDoyDI21.Szt0w8V1g0jNmS6TYBa2VGHGAcpXHByvG1jYaJ0INIrNM2
cj7kmjtozYJsaoJuLCuctHXaFDaqHw5GbPqN0klNltCF3WG65uMy4gP6dYhb
H9T2RmZ07HNRmD4tzv4KbOAuozkHpxCQzvc7LLZiSBR25jffuBy5IWORw5KE
CagO+YWiuFKOA0VOzDY5zRRqtz4Jszqgz5ZjVWqxRqpTWXei6VWyXx0d4nfB
+8c+C81VE7B
-----------end_max5_patcher-----------


*/

