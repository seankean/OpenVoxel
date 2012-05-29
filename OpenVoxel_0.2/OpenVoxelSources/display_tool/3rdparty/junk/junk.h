/*
  //TODO lets use the same licensing model employed by openni
  ======================================================================
  This is the OpenVoxel command module implementing the command structure of the LightCrafter DLP (???)
  LightCrafter Command protocol source code implementation � VOXON, Co. http:\\www.voxon.co    2012
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
#define HDMIVideoInput 0x02
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

#define CmdCurrentTestPatternType 0x01
#define CmdCurrentTestPatternSub 0x03

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


/*

3.10 Pattern Sequence Setting (0x04 0x00) 
This command defines/reads a pattern sequence setting. 

Table 12. Host Write Command / LightCrafter Read Response Data Payload 

DATA BYTE VALUE -DESCRIPTION 
Byte 0 Pattern bit depth (1 � 8) 
Byte 1 Number of patterns in the sequence ( 1 � 96) 
Byte 2 
Include inverted patterns (only applicable for 1 bit patterns) 
0 � Every pattern is displayed 
1 � Each pattern is displayed and followed by its inverted pattern 
before the next pattern is displayed 
Byte 3 
Input trigger type 
00 � Command Trigger (Through Current Pattern Command) 
01 � Auto Trigger 
02 � External Trigger (Positive Polarity) 
03 � External Trigger (Negative Polarity) 
04 � Camera Trigger (Positive Polarity) 
05 � Camera Trigger (Negative Polarity) 
06 � External Trigger + Exposure (only for 1-bit depth mode) 
Byte 4 -Byte 7 Input Trigger Delay in micro seconds 
Byte 8 -Byte 11 Trigger Period in micro second (only for Auto Trigger mode) 
Byte 12 -Byte 15 Exposure Time in micro seconds 
Byte 16 
LED Select 
0 � Red 
1 � Green 
2 � Blue 

*/
#define CmdPatternSequenceSettingType 0x04
#define CmdPatternSequenceSettingSub 0x00

/*
  3.11 Pattern Definition (0x04 0x01) 
  This command defines/reads the pattern used in the pattern sequence. 

  Table 13. Host Write Command / LightCrafter Read Response Data Payload 

  DATA BYTE VALUE -DESCRIPTION 
  Byte 0 Pattern Number (0 � 95/BitsPerPattern) 
  Byte 1 -Byte N 
  The Pattern bitmap in Windows BMP format. Supported bit-depths 
  are 1, 2, 4 and 8. Only the least bits of size defined by the current bit 
  depth will be used. 

  Table 14. Host Read Command Data Payload 

  DATA BYTE VALUE -DESCRIPTION 
  Byte 0 Pattern Number (0 � 95/BitsPerPattern) 

  Table 15. LightCrafter Read Response Data Payload 

  DATA BYTE VALUE -DESCRIPTION 
  Byte 0 -Byte N 
  The pattern bitmap in Windows BMP format. Supported bit-depths 
  are 1, 2, 4, and 8. If the image bit-depth is more than the current bit-
  depth setting, then only the least significant bits of the image bit-
  depth are used. 
*/


#define CmdPatternDefinitionType 0x04
#define CmdPatternDefinitionSub 0x01 

/*

3.12 Start Pattern Sequence (0x04 0x02) 
This command starts or stops the display of currently defined pattern sequence 

Table 16. Host Write Command / LigthCrafter Read Response Data Payload 

DATA BYTE VALUE -DESCRIPTION 
Byte 0 
0 � Stop Pattern Sequence 
1 � Start Pattern Sequence 

*/

#define CmdStartPatternSequenceType 0x04 
#define CmdStartPatternSequenceSub 0x02 

/*
  3.13 Advance Pattern Sequence (0x04 0x03) 
  This command advances the pattern to the next stored pattern. This is only valid in SW Trigger mode. No 
  data payload is used. 

  Table 17. Host Write Command Data Payload 

  DATA BYTE VALUE -DESCRIPTION 
  none 
*/
#define CmdAdvancePatternSequenceType 0x04 
#define CmdAdvancePatternSequenceSub 0x03
/*
  3.14 Trigger Output Setting (0x04 0x04) 
  This command sets/reads the trigger output setting (used for camera trigger). 

  Table 18. Host Write Command / LigthCrafter Read Response Data Payload 

  DATA BYTE VALUE -DESCRIPTION 
  Byte 0 
  TRUE � Enable Trigger output 
  FALSE � Disable Trigger output 
  Byte 1 Trigger source selection 

  DLP� LightCrafter� DM365 Command Interface Guide DLPU007A�January 2012�Revised March 2012 

  Submit Documentation Feedback 

  Copyright � 2012, Texas Instruments Incorporated 

  
  www.ti.com Command Specification 

  Table 18. Host Write Command / LigthCrafter Read Response Data 
  Payload (continued) 


  DATA BYTE VALUE -DESCRIPTION 
  Byte 2 
  Trigger Polarity 
  0 � Positive 
  1 � Negative 
  Byte 3 -Byte 6 Trigger Delay in micro seconds 
  Byte 7 -Byte 10 Trigger Pulse Width 
*/

#define CmdTriggerOutputSettingType 0x04 
#define CmdTriggerOutputSettingSub 0x04

/*
  3.15 Display Pattern (0x04 0x05) 
  This command (supported in DM365 firmware version 3.0 or greater) continuously displays the selected 
  pattern sequence with the inidcates exposure and trigger period settings. 

  Table 19. Host Write Command Data Payload 

  DATA BYTE VALUE -DESCRIPTION 
  Byte 0 -Byte 1 Pattern number. Range of values 1-1500 

*/

#define CmdDisplayPatternType 0x04
#define CmdDisplayPatternSub 0x05

/*
  3.18 Camera Capture (0x05 0x00) 
  This command captures one frame of the image from the integrated camera port and returns the raw 
  bytes. 

  Table 24. Read Command Data Payload 

  DATA BYTE VALUE -DESCRIPTION 
  Byte 0 
  Capture Mode 
  0 � Stop capture 
  1 -Single frame capture 
  2 -Continuous capture 

  Table 25. Read Response Data Payload 

  DATA BYTE VALUE -DESCRIPTION 
  Bytes 0 -Byte 1 Width of the image 
  Bytes 2 -Byte 3 Height of the image 
  Byte 4 
  Image format 
  0 -RGB888 (3 bytes per pixel) 
  1 -8-bit Gray Scale (1 byte per pixel) 
  2 -10-bit Gray Scale (2 bytes per pixel) 
  3 -UYVY16 (2 bytes per pixel) 
  4 -RGB565 (2 bytes per pixel) 
  5 -SBGGR888 (1 byte per pixel -Bayer format) 
  Bytes 5 -Byte 6 Trigger delay in microseconds 
  Bytes 7 -Byte 10 Trigger pulse width 

  DLP� LightCrafter� DM365 Command Interface Guide DLPU007A�January 2012�Revised March 2012 

  Submit Documentation Feedback 

  Copyright � 2012, Texas Instruments Incorporated 

  
  www.ti.com Command Specification 

*/
#define CmdCameraCaptureType 0x05
#define CmdCameraCaptureSub 0x00
/*

3.19 Save Solution/Read Solution (0x06 0x00) 
This command saves the current settings of the displayed mode in persistent memory storage with the 
given solution name. This setting can be restored using the "Manage Solution" command. 

Table 26. Host Write Command Data Payload 

DATA BYTE VALUE -DESCRIPTION 
Byte 0 -Byte 31 32 byte Solution name (0 padded string) 

Table 27. LightCrafter Read Response Data Payload 

DATA BYTE VALUE -DESCRIPTION 
Byte 0 Number of solutions stored (0 � 254) 
Byte 1 
Default solution index (0 � 254) 
255 � No default solution 
Byte 2 -Byte 33 First solution name (0 padded string) 
Byte 34 -Byte 65 Second solution name (0 padded string) 
� � 
*/

#define CmdSaveSolutionReadSolutionType 0x06
#define CmdSaveSolutionReadSolutionSub 0x00

/*
  3.20 Manage Solution (0x06 0x01) 
  Loads, deletes, or set as default the previously saved solution of the given solution name. 

  Table 28. Host Write Command Data Payload 

  DATA BYTE VALUE -DESCRIPTION 
  Byte 0 
  0 � Delete Solution 
  1 � Load Solution 
  2 � Set as Default Solution 
  Byte 1 -Byte 32 Solution Name (0 padded string) 

*/

#define CmdManageSolutionType 0x06
#define CmdManageSolutionSub 0x01


/*

3.16 Extended Pattern Sequence Setting (0x04 0x08) 
This command (supported in DM365 firmware version 3.0 or greater) defines/reads an extended pattern 
sequence setting. 

Table 20. Host Write Command / LightCrafter Read Response Data Payload 

DATA BYTE VALUE -DESCRIPTION 
Byte 0 Pattern bit depth (1 � 8) 
Byte 1 -Byte 2 Number of patterns in the sequence ( 1 � 1000) 
Byte 3 
Include inverted patterns (only applicable for 1 bit patterns) 
0 � Every pattern is displayed 
1 � Each pattern is displayed and followed by its inverted pattern 
before the next pattern is displayed 
Byte 4 
Input trigger type 
00 � Command Trigger (Through Current Pattern Command) 
01 � Auto Trigger 
02 � External Trigger (Positive Polarity) 
03 � External Trigger (Negative Polarity) 
04 � Camera Trigger (Positive Polarity) 
05 � Camera Trigger (Negative Polarity) 
06 � External Trigger + Exposure (only for 1-bit depth mode) 
Byte 5 -Byte 8 Input Trigger Delay in micro seconds 
Byte 9 -Byte 12 Trigger Period in micro second (only for Auto Trigger mode) 
Byte 13 -Byte 16 Exposure Time in micro seconds 
Byte 17 
LED Select 
0 � Red 
1 � Green 
2 � Blue 
Byte 18 
Play mode (not applicable in Auto Trigger setting) 
0 -Display pattern sequence once, with each pattern displayed after a 
trigger 
1 -Continuously display pattern sequence, with each pattern displayed 
after a trigger 

DLPU007A�January 2012�Revised March 2012 DLP� LightCrafter� DM365 Command Interface Guide 

Submit Documentation Feedback 

Copyright � 2012, Texas Instruments Incorporated 


Command Specification www.ti.com 

*/

#define CmdExtendedPatternSequenceSettingType 0x04
#define  CmdExtendedPatternSequenceSettingSub 0x08

 
/*

3.17 Extended Pattern Definition (0x04 0x81) 
This command (supported in DM365 firmware version 3,0 or greater) defines/reads one pattern of a 
pattern sequence. 

Table 21. Host Write Command Data Payload 

DATA BYTE VALUE -DESCRIPTION 
Byte 0 -Byte 1 Pattern Number (0 � 1500) 
Byte 2 -Byte 3 Column position to display this pattern within the DMD 
Byte 4 -Byte 5 Row position to display this pattern within the DMD 
Byte 6 -Byte N 
The pattern bitmap in Windows BMP format. Supported bit-depths 
are 1, 2, 4, 8, and 24-bits. If the image bit-depth is more than the 
current bit-depth setting, then only the least significant bits of the 
image bit-depth are used. 

Table 22. Host Read Command Data Payload 

DATA BYTE VALUE -DESCRIPTION 
Byte 0 -Byte 1 Pattern Number (0 � 1500) 

Table 23. LightCrafter Read Response Data Payload 

DATA BYTE VALUE -DESCRIPTION 
Byte 0 -Byte N 
The pattern bitmap in Windows BMP format. Supported bit-depths 
are 1, 2, 4, 8, and 24-bits. If the image bit-depth is more than the 
current bit-depth setting, then only the least significant bits of the 
image bit-depth are used. 
*/



static char cmdflags_strings[4][20]=
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

static char  cmdtype_strings[18][32]=
  {
    "CmdGetVersionType"
    ,"CmdDisplayModeType"
    ,"CmdCurrentTestPatternType"
    ,"CmdLEDCurrentSettingType"
    ,"CmdStaticImageType"
    ,"CmdStaticColorType"
    ,"CmdDisplayOrientationType"
    ,"CmdVideoInputSettingType"
    ,"CmdVideoModeSettingType"
    ,"CmdPatternSequenceSettingType"
    ,"CmdPatternDefinitionType"
    ,"CmdStartPatternSequenceType"
    ,"CmdAdvancePatternSequenceType"
    ,"CmdTriggerOutputSettingType"
    ,"CmdDisplayPatternType"
    ,"CmdCameraCaptureType"
    ,"CmdSaveSolutionReadSolutionType"
    ,"CmdManageSolutionType"
  };

static char  cmdsub_strings[18][32]=
  { "CmdGetVersionSub"
    ,"CmdDisplayModeSub"
    ,"CmdCurrentTestPatternSub"
    ,"CmdLEDCurrentSettingSub"
    ,"CmdStaticImageSub"
    ,"CmdStaticColorSub"
    ,"CmdDisplayOrientationSub"
    ,"CmdVideoInputSettingSub"
    ,"CmdVideoModeSettingSub"
    ,"CmdPatternSequenceSettingSub"
    ,"CmdPatternDefinitionSub"
    ,"CmdStartPatternSequenceSub"
    ,"CmdAdvancePatternSequenceSub"
    ,"CmdTriggerOutputSettingSub"
    ,"CmdDisplayPatternSub"
    ,"CmdCameraCaptureSub"
    ,"CmdSaveSolutionReadSolutionSub"
    ,"CmdManageSolutionType"
  };



const static int error_rawdata_strings_count=11;
static char error_rawdata_strings[error_rawdata_strings_count][101] = {
  "Command execution failed with unknown error." 
  ,"Invalid command." 
  ,"Invalid Parameter."
  ,"Out of memory resource."
  ,"Hardware device failure"
  ,"Hardware busy."
  ,"Not Initialized (any of the preconditions for the command is not met)."
  ,"Some object referred by the command is not found. For example, a solution name was not found."
  ,"ChecksumError."
  ,"Packet format error due to insufficient or larger than expected payload size."
  ,"Command continuation error due to incorrect continuation flag."};

const static int getversion_rawdata_strings_count=3;
static char getversion_rawdata_strings[getversion_rawdata_strings_count][10] = {
  "DM365" // 11
  ,"FPGA"
  ,"MSP430"
};

const static int didsplaymode_rawdata_strings_count=14;
static char displaymode_rawdata_strings[didsplaymode_rawdata_strings_count][30]={
  "StaticImage"
  ,"TestPattern"
  ,"VideoInput"
  ,"Reserved"
  ,"Pattern Sequence Display"};

const static int testpattern_rawdata_strings_count=14;
static char  testpattern_rawdata_strings[testpattern_rawdata_strings_count][30]={
   "Check4x8" 
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
  ,"ANSI4x4Check"};

const static int ledcurrentsettings_rawdata_strings_count=6;
static char ledcurrentsettings_rawdata_strings[ledcurrentsettings_rawdata_strings_count][24]= {
  "RedLEDCurrentLO"
  ,"RedLEDCurrentHI"
  ,"GreenLEDCurrentLO"
  ,"GreenLEDCurrentHI"
  ,"BlueLEDCurrentLO"
  ,"BlueLEDCurrentHI" };
  
const static int staticimage_rawdata_strings_count=1;
static char staticimage_rawdata_strings[staticimage_rawdata_strings_count][16]= {
  "StaticImageData" };

const static int staticcolor_rawdata_strings_count=1;
static char staticcolor_rawdata_strings[staticcolor_rawdata_strings_count][27] = {
  "StaticColorByte(00RRGGBB)"};
  
const static int displayorientation_rawdata_strings_count=6;
static char displayorientation_rawdata_strings[displayorientation_rawdata_strings_count][24]={
  "FlipLongAxis OFF"
  ,"FlipLongAxis ON"
  ,"FlipShortAxis OFF"
  ,"FlipShortAxis ON"
  ,"RotNeg90 Off"
  ,"RotNeg90 On"};

const static int videoinputsetting_rawdata_strings_count=12;
static char videoinputsetting_rawdata_strings[videoinputsetting_rawdata_strings_count][28]={
   "InResXLO"
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
   ,"InResActiveHeightHI"};

const static int videomodesetting_rawdata_strings_count=6;
static char videomodesetting_rawdata_strings[videomodesetting_rawdata_strings_count][25]={
   "FrameRate"
   ,"BitDepth"
   ,"LEDALLON"
   ,"LEDREDOnly"
   ,"LEDGREENOnly"
   ,"LEDBLUEOnly"};

const static int patternsequencesetting_rawdata_strings_count=23;
static char patternsequencesetting_rawdata_strings[patternsequencesetting_rawdata_strings_count][40]={
   "PatternBitDepth"
   ,"NumberOfPatterns"
   ,"InvertedPatterns"
   ,"CommandTrigger"
   ,"AutoTrigger "
   ,"PosExternal Trigger"
   ,"NegExternal Trigger"
   ,"PosCamera Trigger"
   ,"NegCamera Trigger"
   ,"ExternalTriggerAndExposure"
   ,"Input Trigger Delay1"
   ,"Input Trigger Delay2"
   ,"Input Trigger Delay3"
   ,"AutoTriggerPeriod1"
   ,"AutoTriggerPeriod2"
   ,"AutoTriggerPeriod3"
   ,"ExposureTime1"
   ,"ExposureTime2"
   ,"ExposureTime3"
   ,"LED Select "
   ,"Red"
   ,"Green"
   ,"Blue" };

const static int patterndefinition_rawdata_strings_count=2;
static char patterndefinition_rawdata_strings[patterndefinition_rawdata_strings_count][20]={
  "PatternNumber"
  ,"PatternBMP"};

const static int startpatternsequence_rawdata_strings_count=2;
static char startpatternsequence_rawdata_strings[startpatternsequence_rawdata_strings_count][30]={
  "StopPatternSequence"
  ,"StartPatternSequence" };

const static int advancepatternsequence_rawdata_strings_advancepatternsequence_rawdata_strings_count=0;

const static int camtriggeroutputsetting_rawdata_strings_count=11;
static char camtriggeroutputsetting_rawdata_strings[camtriggeroutputsetting_rawdata_strings_count][30]={
   "CamTriggerEnableOutput"
   ,"CamTriggerDisableOutput"
   ,"CamTriggerSourceSelection"
   ,"CamTriggerPosPolarity"
   ,"CamTriggerNegPolarity"
   ,"CamTriggerDelay1"
   ,"CamTriggerDelay2"
   ,"CamTriggerDelay3"
   ,"CamTriggerPulseWidth1"
   ,"CamTriggerPulseWidth2"
   ,"CamTriggerPulseWidth3"};

const static int displaypattern_rawdata_strings_count=2;
static char displaypattern_rawdata_strings[displaypattern_rawdata_strings_count][20]={
  "Pattern number1"							
  ,"Pattern number2"};

const static int cameracapture_rawdata_strings_count=19;
static char cameracapture_rawdata_strings[cameracapture_rawdata_strings_count][55]={
  "CamStopCapture"
  ,"CamSingleFrame"
  ,"CamContinuous"
  ,"ImageWidth1"
  ,"ImageWidth2"
  ,"ImageHeight1"
  ,"ImageHeight2"
  ,"ImageFormatRGB888 (3 bytes per pixel)"
  ,"ImageFormat8-bit Gray Scale (1 byte per pixel)"
  ,"ImageFormat10-bit Gray Scale (2 bytes per pixel)"
  ,"ImageFormatUYVY16 (2 bytes per pixel)"
  ,"ImageFormatRGB565 (2 bytes per pixel)"
  ,"ImageFormatSBGGR888 (1 byte per pixel -Bayer format)"
  ,"TriggerDelay1"
  ,"TriggerDelay1"
  ,"TriggerPulseWidth1"
  ,"TriggerPulseWidth2"
  ,"TriggerPulseWidth3"};

const static int savesolutionreadsolution_rawdata_strings_count=2;
static char savesolutionreadsolution_rawdata_strings[savesolutionreadsolution_rawdata_strings_count][30]={
  "NumberOfSolutionsStored"
  ,"DefaultSolutionIndex"};

const static int managesolution_rawdata_strings_count=3;
static char managesolution_rawdata_strings[managesolution_rawdata_strings_count][22] ={
  "DeleteSolution"
  ,"LoadSolution"
  ,"SetAsDefaultSolution"};

// todo
const static int triggeroutputsetting_rawdata_strings_triggeroutputsetting_rawdata_strings_count=0;

const static int ExtendedPatternSequence_rawdata_strings_count=26;
static char ExtendedPatternSequence__rawdata_strings[ExtendedPatternSequence_rawdata_strings_count][30]={
  "BitDepth"
  ,"Pattern number1"
  ,"Pattern number2"
  ,"InvertedPatterns"
   ,"CommandTrigger"
   ,"AutoTrigger "
   ,"PosExternal Trigger"
   ,"NegExternal Trigger"
   ,"PosCamera Trigger"
   ,"NegCamera Trigger"
   ,"ExternalTriggerAndExposure"
   ,"Input Trigger Delay1"
   ,"Input Trigger Delay2"
   ,"Input Trigger Delay3"
   ,"AutoTriggerPeriod1"
   ,"AutoTriggerPeriod2"
   ,"AutoTriggerPeriod3"
   ,"ExposureTime1"
   ,"ExposureTime2"
   ,"ExposureTime3"
   ,"LED Select "
   ,"Red"
   ,"Green"
   ,"Blue"
  ,"PlaySequenceOnce"
  ,"LoopSequenceForever"};




