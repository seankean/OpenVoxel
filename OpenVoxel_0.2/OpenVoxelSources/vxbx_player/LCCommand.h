/*
======================================================================
LightCrafter Command protocol � Voxon.co 2012
This Software is available to the public under the BSD standard licencse.   
======================================================================
Referenced comments and LightCrafter communication protocol are  
Copyright � 2012, Texas Instruments Incorporated taken from the 
DLP� LightCrafter� DM365 Command Interface Guide DLPU007A�January 2012�Revised March 2012 
======================================================================
Command Specification 

HEADER DATA CHECKSUM 
Byte 0 		Byte 1 Byte 2 Byte 3 Byte 4 Byte 5 Byte 6 .. Byte N Byte (N+1) 
Packet Type 	CMD1  CMD2 Flags Payload Length Data Payload Checksum 

The first byte defines the type of the packet: 
0x00 � LightCrafter System Busy Packet 
0x01 � LightCrafter Error Packet 
0x02 � Host Write Command Packet 
0x03 � LightCrafter Write Response Packet 
0x04 � Host Read Command Packet 
0x05 � LightCrafter Read Response Packet 

*/
#define PType 0
#define CmdType 1
#define CmdSub 2
#define CmdFlags 3
#define DataLenLO 4
#define DataLenHI 5
#define RawData 6

#define SystemBusyPType 0x00

/*
  2.1.1 LightCrafter System Busy Packet [Packet Type 0x00] 
  LightCrafter may send a System Busy Packet as a response to a Host's Write/Read command. It indicates 
  that the LightCrafter system has not completed the previous command or is busy performing some other 
  operation. This response has no data, so the data length of this packet is zero. All the bytes in the packet, 
  except the first one, are ignored. 
*/


#define ErrorPType 0x01

/*

2.1.2 LightCrafter Error Packet [Packet Type 0x01] 
LightCrafter may send an Error Packet as a response to a Host's Write/Read command. It indicates that 
there is an error in interpreting/executing the command. The error is passed in the data payload as one 
byte error code. There can be more than one error code in a single packet. 

The error packets are: 

0x01 � Command execution failed with unknown error 
0x02 � Invalid command 
0x03 � Invalid Parameter 
0x04 � Out of memory resource 
0x04 � Hardware device failure 
0x05 � Hardware busy 
0x06 � Not Initialized (any of the preconditions for the command is not met) 
0x07 � Some object referred by the command is not found. For example, a solution name was not found. 
0x08 � Checksum Error 
0x09 � Packet format error due to insufficient or larger than expected payload size 
0x0A � Command continuation error due to incorrect continuation flag 

*/

#define ExecutionErrorUnknown  0x01 
#define InvalidCmd 0x02 
#define InvalidParam 0x03 
#define MemExhausted 0x04 
#define DeviceFailure 0x04 
#define DeviceBusy 0x05
#define MissingValue 0x06
#define ObjectNotFound 0x07
#define ChecksumError 0x08 
#define DataSizeMismatch 0x09 
#define BadContinue 0x0A 

#define WriteCommandPType 0x02

/*

2.1.3 Host Write Command Packet [Packet Type 0x02] 
A host Write Command sends configuration information to the LightCrafter module. The data payload 
contains the parameters of the command. 

*/
#define WriteResponsePType 0x03
/*

2.1.4 LightCrafter Write Response Packet [Packet Type 0x03] 
The LightCrafter sends a write response packet to the host to indicate successful execution. The CMD1 
and CMD2 are same as the corresponding Write Command. This packet has no data payload. 

*/

#define ReadCommandPType 0x04

/*

2.1.5 Host Read Command Packet [Packet Type 0x04] 
A host Read Command requests information from the LightCrafter module. The information requested is 
identified using the CMD2 and CMD3. Any additional parameter is passed in the data payload. 

*/

#define ReadResponsePType 0x05 

/*
  2.1.6 LightCrafter Read Response Packet [Packet Type 0x05] 
  The LightCrafter sends a Read Response packet to the host as a response to a host Read Command. 
  The CMD1 and CMD2 are same as the corresponding read command. The data payload has non-zero 
  length and contains the requested information. 

  DLPU007A�January 2012�Revised March 2012 DLP� LightCrafter� DM365 Command Interface Guide 

  Submit Documentation Feedback 

  Copyright � 2012, Texas Instruments Incorporated 

  
  Command Specification www.ti.com 


  2.2 Command ID (CMD1 & CMD2) 
  CMD1 and CMD2 together form a unique ID for the command. The list of commands supported by the 
  LightCrafter module is explained in Section 3 

  The CMD1 identifies the main command number and CMD2 describes the sub command number. 

  2.3 Payload Length (L1 & L2) 
  The bytes L1 and L2 represent the data length excluding checksum. L1 corresponds to the LSB while L2 
  corresponds to the MSB. 

  2.4 Data Payload 
  The data related to the currentCurrent Test Pattern command. The data length and content will vary depending on the 
  command. 
The maximum data payload size is 65535 bytes. 

*/

#define MaxRawDataSize 65535 

/*

  2.5 Checksum 
  This byte is used for verifying the integrity of the command packet. It is the sum of all the bytes in the 
  current packet, not including the checksum byte. 

  Checksum = (Byte 0 + Byte 1 + � Byte N) MODULO[ 0x100 ] 

  2.6 Command Flags 
  The command interpreter utilizes the flags to understand and follow the transfer of data through multiple 
  command and response packets. The flags are: 
  0x00 � The packet payload contains the complete data 
  0x01 � The packet payload contains the beginning of the data 
  0x02 � The packet payload contains the intermediate data 
  0x03 � The packet payload contains the last data 

*/

#define DataTransferAll 0x00 
#define DataTransferFirst 0x01 
#define DataTransferNext 0x02 
#define DataTransferLast 0x03


/*

3 Command Specification 

The following sections define the list of commands and their corresponding data payloads as supported by 
the LightCrafter module. The command number is the hex representation of CMD1 and CMD2 bytes. All 
the multi-byte data are sent in Little-endian order. 
*/
#define CmdGetVersionType 0x01
#define CmdGetVersionSub 0x0
 
/*
  3.1 Version String (0x01 0x00) 
  This command reads the LightCrafter software and firmware versions. 

  Table 1. Host Read Command Data Payload 

  DATA BYTE VALUE -DESCRIPTION 
  Byte 0 

  0x00 � DM365 SW Revision 
  0x10 � FPGA Firmware Revision 
  0x20 � MSP430 SW Revision 

*/

#define DM365 0x00 
#define FPGA 0x10 
#define MSP430 0x20 

/*
  Table 2. LightCrafter Read Response Data Payload 

  DATA BYTE VALUE -DESCRIPTION 
  Byte 0 � Byte 31 Revision String (Null terminated ASCII string) 


  DLP� LightCrafter� DM365 Command Interface Guide DLPU007A�January 2012�Revised March 2012 

  Submit Documentation Feedback 

  Copyright � 2012, Texas Instruments Incorporated 

  
  www.ti.com Command Specification 

*/
#define CmdDisplayModeType 0x01
#define CmdDisplayModeSub 0x01
/*

3.2 Current Display Mode (0x01 0x01) 


This command sets/reads the current display mode. 

Table 3. Host Write Command / LightCrafter Read Response Data Payload 

DATA BYTE VALUE -DESCRIPTION 
Byte 0 

0x00 � Static Image 
0x01 � Internal Test Pattern 
0x02 � HDMI Video Input 
0x03 -Reserved 
0x04 -Pattern Sequence Display 

*/

#define StaticImage 0x00
#define TestPattern 0x01
#define VideoInput 0x02
#define Reserved 0x03
#define PatternSequence 0x04

/*
  3.3 Current Test Pattern (0x01 0x03) 
  This command sets/reads the currently selected Internal Test Pattern. 

  Table 4. Host Write Command / LightCrafter Read Response Data Payload 

  DATA BYTE VALUE -DESCRIPTION 
  Byte 0 

  0x00 -14x8 Checkerboard (default) 
  0x01 -Solid black 
  0x02 -Solid white 
  0x03 -Solid green 
  0x04 -Solid blue 
  0x05 -Solid red 
  0x06 -Vertical lines (1-white, 7-black) 
  0x07 -Horizontal lines (1-white, 7-black) 
  0x08 -Vertical lines (1-white, 1-black) 
  0x09 -Horizontal lines (1-white, 1-black) 
  0x0A -Diagonal lines 
  0x0B -Vertical Gray Ramps 
  0x0C -Horizontal Gray Ramps 
  0x0D -ANSI 4x4 Checkerboard 
*/

#define CurrentTestPatternType 0x01
#define CurrentTestPatternSub 0x03

#define Check4x8 0x00 
#define SolidBlack 0x01 
#define SolidWhite 0x02 
#define SolidGreen 0x03 
#define SolidBlue 0x04 
#define SolidRed 0x05 
#define VerticalLinesw1b7 0x06 
#define HorizontalLinesw1b7 0x07 
#define VerticalLinesw1b1 0x08 
#define HorizontalLinesw1b1 0x09 
#define DiagonalLines 0x0A 
#define VerticalGrayRamps 0x0B 
#define HorizontalGrayRamps 0x0C 
#define ANSI4x4Check 0x0D 

#define CmdLEDCurrentSettingType 0x01
#define CmdLEDCurrentSettingSub 0x04 

/*
  3.4 LED Current Setting (0x01 0x04) 
  This command sets/reads the red, green, and blue LED current values. 

  Table 5. Host Write Command / LightCrafter Read Response Data Payload 

  DATA BYTE VALUE -DESCRIPTION 

  Byte 0 Red LED current. Range 0 � 1024. Default 274 
  Byte 1 Red LED current. Range 0 � 1024. Default 274 
  Byte 2 Green LED current. Range 0 � 1024. Default 274 
  Byte 3 Green LED current. Range 0 � 1024. Default 274 
  Byte 4 Blue LED current. Range 0 � 1024. Default 274 
  Byte 5 Blue LED current. Range 0 � 1024. Default 274 

*/

#define RedLEDCurrentLO 0x00
#define RedLEDCurrentHI 0x01
#define GreenLEDCurrentLO 0x02
#define GreenLEDCurrentHI 0x03
#define BlueLEDCurrentLO 0x04
#define BlueLEDCurrentHI 0x05

/*
  If 0 is set, then that particular LED is disabled. 0x400 means full current (2A) and should only be used in 
  short bursts. A value of 0x112 corresponds to 633mA. 633mA is the maximum amount for continuous 
  operation with no active thermal cooling. 
*/

#define CmdStaticImageType 0x01
#define CmdStaticImageSub 0x05

/*


3.5 Static Image (0x01 0x05) 
This command loads a static bitmapped image into the LightCrafter's DLPC300 memory buffer. 

DLPU007A�January 2012�Revised March 2012 DLP� LightCrafter� DM365 Command Interface Guide 

Submit Documentation Feedback 

Copyright � 2012, Texas Instruments Incorporated 


Command Specification www.ti.com 

Table 6. Host Write Command Data Payload 

DATA BYTE VALUE -DESCRIPTION 

Byte 0 -Byte N BMP image file contents. 

*/
 
#define CmdStaticColorType 0x01
#define CmdStaticColorSub 0x06
 
/* 

This command fills the screen with given color (24 bit) when set to "Static Image mode". 

Table 7. Host Write Command Data Payload 

DATA BYTE VALUE -DESCRIPTION 
Byte 0 -Byte 3 24 bit Color in 00RRGGBB format 
*/

//Byte 0 -Byte 3 24 bit Color in 00RRGGBB format 


#define CmdDisplayOrientationType 0x01
#define CmdDisplayOrientationSub 0x07


/*

3.7 Display Setting (0x01 0x07) 
This command sets/reads the current display setting. 

Table 8. Host Write Command / LightCrafter Read Response Data Payload 

Byte 0 
0 -No flip 
1 -Flip on long axis 
Byte 1 
0 -No flip 
1 -Flip on short axis
Byte 2 
0 -No rotation
1 -Rotate
*/

#define FlipLongAxis 0x00
#define VOff 0x00 
#define VOn 0x01

#define FlipShortAxis 0x00
//von voff value define used again here

#define RotNeg90 0x00
//von voff value define used again here


/*
  3.8 Video Input Setting (0x02 0x00) 
  This command sets/reads the current video input setting. 

  Table 9. Host Write Command / LightCrafter Read Response Data Payload 

  DATA BYTE VALUE -DESCRIPTION 

  Byte 0 Input Resolution X 
  Byte 1 Input Resolution X 
  Byte 2 Input Resolution Y 
  Byte 3 Input Resolution Y 
  Byte 4 First Active Pixel (Input cropping) 
  Byte 5 First Active Pixel (Input cropping) 
  Byte 6 First Active Line (Input cropping) 
  Byte 7 First Active Line (Input cropping) 
  Byte 8 Active Width (Input cropping) 
  Byte 9 Active Width (Input cropping) 
  Byte 10 Active Height (Input cropping) 
  Byte 11 Active Height (Input cropping) 
*/

#define CmdVideoInputSettingType 0x02
#define CmdVideoInputSettingSub 0x00


#define InResXLO 0 
#define InResXHI 1 
#define InResYLO 2 
#define InResYHI 3 
#define InResFirstActivePixelLO 4 
#define InResFirstActivePixelHI 5
#define InResFirstActiveLineLO 6
#define InResFirstActiveLineHI 7
#define InResActiveWidthLO  8
#define InResActiveWidthHI 9
#define InResActiveHeightLO 10
#define InResActiveHeightHI 11

/*
  Table 10 shows the supported resolutions. Note that the EDID is set to 608 x 684 mode. To report a 
  different resolution through the HDMI input, a different resolution must be programmed on the EDID. 

  Table 10. Supported Resolutions for Video Input 

  240x320 640x480 480x852 864x480 
  320x240 480x720 852x480 720x240 
  240x427 720x480 480x853 720x288 
  427x240 480x752 853x480 360x640 

  DLP� LightCrafter� DM365 Command Interface Guide DLPU007A�January 2012�Revised March 2012 

  Submit Documentation Feedback 

  Copyright � 2012, Texas Instruments Incorporated 

  
  www.ti.com Command Specification 

  Table 10. Supported Resolutions for Video 
  Input (continued) 


  430x640 752x480 480x853 640x360 
  640x430 480x800 854x480 854x480 
  480x640 800x480 480x854 608x684 

  3.9 Video Mode Setting (0x02 0x01) 
  This command sets/reads the current video mode setting. 

  Table 11. Host Write Command / LightCrafter Read Response Data Payload 

  DATA BYTE VALUE -DESCRIPTION 

  Byte 0 Frame Rate 15, 30, 40, 50, or 60 Hz. Default is 60 Hz 
  Byte 1 Bit Depth. Range 1 � 8. (default is 8) 
  Byte 2 
  1 -RGB: Uses Red, Green, and Blue LEDs for full color (default) 
  2 -Monochrome Red LED 
  3 -Monochrome Green LED 
  4 -Monochrome Blue LED 
*/

#define CmdVideoModeSettingType 0x02 
#define CmdVideoModeSettingSub 0x01 
#define FrameRate 0
#define BitDepth 1
#define Colormode 2

static char  cmdflags_strings[4][20]=
{"DataTransferAll"
 ,"DataTransferFirst"
 ,"DataTransferNext"
 ,"DataTransferLast"
};

static char ptype_strings[6][30]=
  {"SystemBusyPacket"
   ,"ErrorPacket"
   ,"WriteCommandPacket"
   ,"WriteResponsePacket"
   ,"ReadCommandPacket"
   ,"ReadResponsePacket"};

static char  cmdtype_strings[9][30]=
  {
    "CmdGetVersionType"
    ,"CmdDisplayModeType"
    ,"CurrentTestPatternType"
    ,"CmdLEDCurrentSettingType"
    ,"CmdStaticImageType"
    ,"CmdStaticColorType"
    ,"CmdDisplayOrientationType"
    ,"CmdVideoInputSettingType"
    ,"CmdVideoModeSettingType"
  };

static char  cmdsub_strings[10][30]=
  { "CmdGetVersionSub"
    ,"CmdDisplayModeSub"
    ,"CmdCurrentTestPatternSub"
    ,"CmdLEDCurrentSettingSub"
    ,"CmdStaticImageSub"
    ,"CmdStaticColorSub"
    ,"CmdDisplayOrientationSub"
    ,"CmdVideoInputSettingSub"
    ,"CmdVideoModeSettingSub"};

static int error_rawdata_strings_count=10;
static int getversion_rawdata_strings_count=3;
static int displaymode_rawdata_strings_count=5;
static int testpattern_rawdata_strings_count=14;
static int ledcurrentsettings_rawdata_strings_count=6;
static int staticimage_rawdata_strings_count=1;
static int staticcolor_rawdata_strings_count=1;
static int displayorientation_rawdata_strings_count=6;
static int videoinputsetting_rawdata_strings_count=12;
static int videomodesetting_rawdata_strings_count=3;

static char  rawdata_strings[66][101]=
  {"Command execution failed with unknown error." 
   ,"Invalid command." 
   ,"Invalid Parameter."
   ,"Out of memory resource."
   ,"Hardware device failure"
   ,"Hardware busy."
   ,"Not Initialized (any of the preconditions for the command is not met)."
   ,"Some object referred by the command is not found. For example, a solution name was not found."
   ,"ChecksumError."
   ,"Packet format error due to insufficient or larger than expected payload size."
   ,"Command continuation error due to incorrect continuation flag."
   ,"DM365" // 11
   ,"FPGA"
   ,"MSP430"
   ,"StaticImage" //14 
   ,"TestPattern"
   ,"VideoInput"
   ,"Reserved"
   ,"Pattern Sequence Display"
   ,"Check4x8" //19
   ,"SolidBlack"
   ,"SolidWhite"
   ,"SolidGreen"
   ,"SolidBlue"
   ,"SolidRed"
   ,"VerticalLinesw1b7"
   ,"HorizontalLinesw1b7"
   ,"VerticalLinesw1b1"
   ,"HorizontalLinesw1b1"
   ,"DiagonalLines"
   ,"VerticalGrayRamps"
   ,"HorizontalGrayRamps"
   ,"ANSI4x4Check"
   ,"RedLEDCurrentLO"
   ,"RedLEDCurrentHI"
   ,"GreenLEDCurrentLO"
   ,"GreenLEDCurrentHI"
   ,"BlueLEDCurrentLO"
   ,"BlueLEDCurrentHI"
   ,"StaticImageData"
   ,"StaticColorByte(00RRGGBB)"
   ,"FlipLongAxis OFF"
   ,"FlipLongAxis ON"
   ,"FlipShortAxis OFF"
   ,"FlipShortAxis ON"
   ,"RotNeg90 Off"
   ,"RotNeg90 On"
   ,"InResXLO"
   ,"InResXHI"
   ,"InResYLO"
   ,"InResYHI"
   ,"InResFirstActivePixelLO"
   ,"InResFirstActivePixelHI"
   ,"InResFirstActiveLineLO"
   ,"InResFirstActiveLineHI"
   ,"InResActiveWidthLO"
   ,"InResActiveWidthHI"
   ,"InResActiveHeightLO"
   ,"InResActiveHeightHI"
   ,"FrameRate"
   ,"BitDepth"
   ,"LEDColormode"
   ,"LEDRGBAllON"
   ,"LEDREDOnly"
   ,"LEDGREENOnly"
   ,"LEDBLUEOnly"

  };

// NO PATTERNs No PATTERNEX no SolutionSave/load NO firmware upgrade...