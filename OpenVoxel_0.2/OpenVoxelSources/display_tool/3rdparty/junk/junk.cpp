#include "junk.h"
#include <QDebug>

char checksum(char *b,int len)
{
  char c=0;
  for(int i=0;i<len;i++)
    c+=b[i];
  return c;
}

QString * cmd_to_string(char *msg)
{
  QString *str = new QString(); 
  int i=0;
  int s=0;
  
  // if we are making a write command with a buffer we store char * of the data at byte 6 and the checksum in byte 10
  // for read we treat it as documented

  // output the bytes in hex
  *str+="HEX:";
  
  for(i=0;i<6;i++)
    {
      *str += '[';
      *str += QByteArray(1,msg[i]).toHex();
      *str += ']';
    }
  *str+='\t';
  *str+= ptype_strings[msg[PType]];
  *str+=':';
  if( 
     msg[PType]==SystemBusyPType ||  
     msg[PType]==WriteResponsePType  
     )
    {
      return str;
    }
  if(msg[PType]==ErrorPType)
    {
      *str+=error_rawdata_strings[msg[RawData]];
      return str;
    }
  *str+=cmdtype_strings[msg[CmdType]-1];
  *str+=':';
  *str+=cmdsub_strings[msg[CmdSub]];
  *str+=':';
  *str+=cmdflags_strings[msg[CmdFlags]];
  *str+=':';
  // all others are "command based"

  //  make the commandtype,subtype a unique number to map to the data decription strings
  int cmdnum=(msg[CmdType]*256)+msg[CmdSub];
  switch(cmdnum)
    {
    case  (CmdGetVersionType*256)+CmdGetVersionSub:
      if(msg[PType]==WriteCommandPType)
	{
	  *str += getversion_rawdata_strings[msg[RawData]];
	}
      else if (msg[PType]==ReadCommandPType)
	{
	  *str += (char*)&msg[RawData];
	}
      *str+=':';
      break;
    case (CmdDisplayModeType *256)+CmdDisplayModeSub:
      *str += displaymode_rawdata_strings[msg[RawData]];
      *str+=':';
      break;
    case (CmdCurrentTestPatternType * 265)+CmdCurrentTestPatternSub:
      *str += testpattern_rawdata_strings[msg[RawData]];
      *str+=':';
      break;
    case (CmdLEDCurrentSettingType *256)+CmdLEDCurrentSettingSub:
      for(i=s;i<(ledcurrentsettings_rawdata_strings_count);i++)
	{
	  *str += ledcurrentsettings_rawdata_strings[i];
	  *str += QString("=%1:").arg( msg[RawData+(i)] );
	}
      break;
    case (CmdStaticImageType *256)+CmdStaticImageSub:
      *str += staticimage_rawdata_strings[msg[RawData]];
      if(msg[PType]==WriteCommandPType)
	{ 
	  *str += ":ADDR=";
	  *str += &msg[RawData];
	}
      *str+=':';
      break;
    case (CmdStaticColorType *256)+CmdStaticColorSub:
      *str += staticcolor_rawdata_strings[msg[RawData]];
      *str += QString("=%1:").arg(msg[(RawData+i)]);
      break;
    case (CmdDisplayOrientationType *256)+CmdDisplayOrientationSub:
      for(i=s;i<((displayorientation_rawdata_strings_count/2));i++) // three bytes with two choices of 0 or 1
	{
	  *str += displayorientation_rawdata_strings[(i*2)+msg[RawData+i] ];
	  *str+=':';
	}
      break;
    case (CmdVideoInputSettingType *256)+CmdVideoInputSettingSub:

      for(i=s;i<(videoinputsetting_rawdata_strings_count);i++)
	{
	  *str += videoinputsetting_rawdata_strings[i];
	  *str += QString("=%1:").arg(msg[RawData+i]);
	}
      break;
    case (CmdVideoModeSettingType *256)+CmdVideoModeSettingSub:
      *str += videomodesetting_rawdata_strings[msg[RawData]];
      *str+=':';
      break;
    case (CmdPatternSequenceSettingType *256)+CmdPatternSequenceSettingSub:
      
      for(i=s;i<(patternsequencesetting_rawdata_strings_count);i++)
	{
	  *str += patternsequencesetting_rawdata_strings[i];
	  *str += QString("=%1:").arg(msg[(RawData+(i))]);
	}
      break;
    case CmdPatternDefinitionType * 256  + CmdPatternDefinitionSub:

      if(msg[PType]==WriteCommandPType ||
	 msg[PType]==WriteResponsePType)
	{
	  *str += patterndefinition_rawdata_strings[s];
	  *str+=':';
	  *str += patterndefinition_rawdata_strings[1];
	  *str+=':';
	  *str += "ADDR=";
	  *str += &msg[RawData];
	  *str+=':';
	}
      else if (msg[PType]==ReadCommandPType )
	{
	  *str += patterndefinition_rawdata_strings[s];
	  *str+=':';
	}
      else if(msg[PType]==ReadResponsePType)
	{
	  *str += patterndefinition_rawdata_strings[1];
	  *str+=':';
	  *str += "ADDR=";
	  *str += &msg[RawData];
	  *str+=':';
	}
      break;       
    case (CmdStartPatternSequenceType *256)+CmdStartPatternSequenceSub:
      *str += startpatternsequence_rawdata_strings[msg[RawData]];
      *str+=':';
      break;
    case (CmdAdvancePatternSequenceType *256) + CmdAdvancePatternSequenceSub:
      break;
    case ( CmdTriggerOutputSettingType * 256) + CmdTriggerOutputSettingSub:

      *str += camtriggeroutputsetting_rawdata_strings[msg[RawData]];
      *str+=':';
      *str += camtriggeroutputsetting_rawdata_strings[s];
      *str += QString("=%1:").arg(msg[RawData+1]);
      *str += camtriggeroutputsetting_rawdata_strings[msg[RawData+2]];
      *str+=':';
      for(i=s;i<(6);i++)
	{
	  *str += camtriggeroutputsetting_rawdata_strings[s];
	  *str += QString("=%1:").arg(msg[RawData+3+(i)]);
	}
      break;
    case (CmdDisplayPatternType * 256 ) + CmdDisplayPatternSub:
      for(i=s;i<(2);i++)
	{
	  *str += displaypattern_rawdata_strings[s];
	  *str += QString("=%1:").arg(msg[RawData+(i)]);
	}
      break;
    case (CmdCameraCaptureType * 256) * CmdCameraCaptureSub:
      if (msg[PType]==ReadCommandPType )
	{
	  *str += cameracapture_rawdata_strings[msg[RawData]];
	  *str+=':';
	}
      else  if (msg[PType]==ReadResponsePType )
	{
	  //	  =3;
	  for(i=s;i<(4);i++)
	    {
	      *str += cameracapture_rawdata_strings[s];
	      *str += QString("=%1:").arg(msg[RawData+(i)]);
	    }
	  // =4;
	  *str += cameracapture_rawdata_strings[msg[RawData]];
	  *str+=':';
	  // +;
	  for(i=s;i<(5);i++)
	    {
	      *str += cameracapture_rawdata_strings[s];
	      *str += QString("=%1:").arg(msg[RawData+(i)]);
	    }
	}
      break;
    case (CmdSaveSolutionReadSolutionType * 256) + CmdSaveSolutionReadSolutionSub:
      if(msg[PType]==WriteCommandPType)
	{
	  *str += msg[RawData]; // writing out it is a pointer
	}
      else if (msg[PType]==ReadResponsePType)
	{
	  //	  =2;
	  *str += savesolutionreadsolution_rawdata_strings[s];
	  *str += QString("=%1:").arg(msg[RawData]);
	  //+;
	  *str += savesolutionreadsolution_rawdata_strings[s];
	  *str += QString("=%1:").arg(msg[RawData+1]);
	  *str += msg[RawData+2]; // reading in it is char[32]
	  *str += msg[RawData+34]; // reading in it is char[32]
	}
      break;
    case (CmdManageSolutionType * 256) + CmdManageSolutionSub:
      *str += managesolution_rawdata_strings[msg[RawData]];
      *str+=":";
      *str += msg[RawData+1]; // writing out it is a pointer
      break;
    default:
      // complain
      *str+="MALFORMED COMMAND";
      break;
    }
  return str;
}


int main()
{
  char out_msg[6+sizeof(char*)+1];
  char *in_msg =0;
  //  char bigbuff[100];
  char *c = new char[1];
  // get the DM365 version
  qDebug() << "create get the DM365 version command";
  out_msg[PType]=WriteCommandPType;
  out_msg[CmdType]=CmdGetVersionType;
  out_msg[CmdSub]=CmdGetVersionSub;
  out_msg[CmdFlags]=DataTransferAll;
  out_msg[DataLenLO]=1;
  out_msg[DataLenHI]=0;
  out_msg[RawData]=(*c=DM365);
  out_msg[RawData+1]=checksum(c,1);
  qDebug() << *cmd_to_string(out_msg);

  qDebug() << "create Good Response to: get the DM365 version command";
  in_msg=new char [40];
  in_msg[PType]=ReadCommandPType;
  in_msg[CmdType]=CmdGetVersionType;
  in_msg[CmdSub]=CmdGetVersionSub;
  in_msg[CmdFlags]=DataTransferAll;
  in_msg[DataLenLO]=33;
  in_msg[DataLenHI]=0;
  char * fakever="Fake Version String is 33 chars.\0";
  strcpy(&in_msg[RawData],fakever);
  qDebug() << *cmd_to_string(in_msg);
  delete in_msg;
  qDebug() << "create ERROR Response to: get the DM365 version command";
  in_msg = new char [8];
  in_msg[PType]=ErrorPType;
  in_msg[CmdType]=0;
  in_msg[CmdSub]=0;
  in_msg[CmdFlags]=0;
  in_msg[DataLenLO]=0;
  in_msg[DataLenHI]=0;
  in_msg[RawData]= 0;
  in_msg[RawData+1]=0;
  qDebug() << *cmd_to_string(in_msg);
  //still 8
  qDebug() << "create SYSTEM_BUSY Response to: get the DM365 version command";
  in_msg[PType]=SystemBusyPType;
  in_msg[CmdType]=0;
  in_msg[CmdSub]=0;
  in_msg[CmdFlags]=0;
  in_msg[DataLenLO]=0;
  in_msg[DataLenHI]=0;
  in_msg[RawData]= 0;
  in_msg[RawData+1]=0;
  qDebug() << *cmd_to_string(in_msg);
 
  qDebug() << "create Read DisplayMode command";
  out_msg[PType]=ReadCommandPType;
  out_msg[CmdType]=CmdDisplayModeType;
  out_msg[CmdSub]=CmdDisplayModeSub;
  out_msg[CmdFlags]=DataTransferAll;
  out_msg[DataLenLO]=1;
  out_msg[DataLenHI]=0;
  // out_msg[RawData]=HDMIVideoInput;
  // out_msg[RawData+1]=checksum(c,1);
  qDebug() << *cmd_to_string(out_msg);

  qDebug() << "create Write DisplayMode:HDMI: command";
  out_msg[PType]=WriteCommandPType;
  out_msg[CmdType]=CmdDisplayModeType;
  out_msg[CmdSub]=CmdDisplayModeSub;
  out_msg[CmdFlags]=DataTransferAll;
  out_msg[DataLenLO]=1;
  out_msg[DataLenHI]=0;
  out_msg[RawData]=HDMIVideoInput;
  out_msg[RawData+1]=checksum(c,1);
  qDebug() << *cmd_to_string(out_msg);


  qDebug() << "create Read InternalTestPattern: command";
  out_msg[PType]=ReadCommandPType;
  out_msg[CmdType]=CmdCurrentTestPatternType;
  out_msg[CmdSub]=CmdCurrentTestPatternSub;
  out_msg[CmdFlags]=DataTransferAll;
  out_msg[DataLenLO]=1;
  out_msg[DataLenHI]=0;
  out_msg[RawData]=VerticalLinesw1b7;
  out_msg[RawData+1]=checksum(c,1);
  qDebug() << *cmd_to_string(out_msg);

 qDebug() << "create Write InternalTestPattern: command";
  out_msg[PType]=WriteCommandPType;
  out_msg[CmdType]=CmdCurrentTestPatternType;
  out_msg[CmdSub]=CmdCurrentTestPatternSub;
  out_msg[CmdFlags]=DataTransferAll;
  out_msg[DataLenLO]=1;
  out_msg[DataLenHI]=0;
  out_msg[RawData]=VerticalLinesw1b7;
  out_msg[RawData+1]=checksum(c,1);
  qDebug() << *cmd_to_string(out_msg);

 qDebug() << "create Write InternalLEDCurrentSetting: command";
  out_msg[PType]=WriteCommandPType;
  out_msg[CmdType]=CmdLEDCurrentSettingType;
  out_msg[CmdSub]=CmdLEDCurrentSettingSub;
  out_msg[CmdFlags]=DataTransferAll;
  out_msg[DataLenLO]=6;
  out_msg[DataLenHI]=0;
  int val=273;
  unsigned char LO=0,HI=0;
  HI=val/256;
  LO=val - (HI*256);
  out_msg[RawData + RedLEDCurrentLO ] = LO;
  out_msg[RawData + RedLEDCurrentHI ] = HI;
  out_msg[RawData + GreenLEDCurrentLO ] = LO;
  out_msg[RawData + GreenLEDCurrentHI ] = HI;
  out_msg[RawData + BlueLEDCurrentLO ] = LO;
  out_msg[RawData + BlueLEDCurrentHI ] = HI;
  out_msg[RawData+ 1 + BlueLEDCurrentHI ]=checksum(c,1);
  qDebug() << *cmd_to_string(out_msg);

 qDebug() << "create Read InternalLEDCurrentSetting: command";
  out_msg[PType]=ReadCommandPType;
  out_msg[CmdType]=CmdLEDCurrentSettingType;
  out_msg[CmdSub]=CmdLEDCurrentSettingSub;
  out_msg[CmdFlags]=DataTransferAll;
  out_msg[DataLenLO]=6;
  out_msg[DataLenHI]=0;
  val=273;
  HI=val/256;
  LO=val - (HI*256);
  out_msg[RawData + RedLEDCurrentLO ] = LO;
  out_msg[RawData + RedLEDCurrentHI ] = HI;
  out_msg[RawData + GreenLEDCurrentLO ] = LO;
  out_msg[RawData + GreenLEDCurrentHI ] = HI;
  out_msg[RawData + BlueLEDCurrentLO ] = LO;
  out_msg[RawData + BlueLEDCurrentHI ] = HI;
  out_msg[RawData+1+BlueLEDCurrentHI]=checksum(c,1);
  qDebug() << *cmd_to_string(out_msg);





  //
  delete in_msg;


  
}


