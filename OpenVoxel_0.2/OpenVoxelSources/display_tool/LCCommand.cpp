#include <QDebug>
#include "LCCommand.h"

// encode the header
// int encodPacketHeader(char ptype,char cmdtype,char cmdsub,char cmdflags,char *head,char dlenlo=0,char=0,char *databuff=0)
// {
//   unsigned short dchecksum=0;

//   msg[PType]=ptype;
//   msg[CmdType]=cmdtype;
//   msg[CmdSub]=cmdsub;
//   msg[CmdFlags]=cmdflags;
//   msg[DataLenLO]=dlenlo;
//   msg[DataLenHI:]=dlenhi;
  
//   if(databuf)
//     {
//       dchecksum=calcChecksum(databuf);
//     }
//   memcpy((char*)(msg+6),buff,cmdlen-7);
//   delete buff[];
//   buff=msg;
//   msg=0;
//   return cmdlen;
// }

// char * makeVersionStringCmd(char *device)
// {
  
// }

// int encodeCmd(char ptype,char cmdtype,char cmdsub,char cmdflags,char *head,int dlen,char *databuff)
// {
  

// // six bytes + checksum byte = 7
//   int cmdlen = 7+ dlenlo + 256*dlenhi;
  

//   char *msg = new char[cmdlen+7];
// }

// int decodeCMD()
// {

// }

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
  
  // if we are making a write command with a buffer we store char * of the data at byte 6 and the checksum in byte 10
  // for read we treat it as documented

  *str+= ptype_strings[msg[PType]];
  *str+=':';
  *str+=cmdtype_strings[msg[CmdType]-1];
  *str+=':';
  *str+=cmdsub_strings[msg[CmdSub]];
  *str+=':';
  *str+=cmdflags_strings[msg[CmdFlags]];
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
      *str+=rawdata_strings[msg[RawData]-1];
      return str;
    }
  // all others are "command based"

  //  make the commandtype,subtype a unique number to map to the data decription strings
  int dstringoffset=(msg[CmdType]*256)+msg[CmdSub];
  int s=error_rawdata_strings_count+1;
  int i=0;
  switch(dstringoffset)
    {
    case  (CmdGetVersionType*256)+CmdGetVersionSub:
      if(msg[PType]==WriteCommandPType)
	{
	  *str += rawdata_strings[msg[RawData]+s];
	}
      else if (msg[PType]==ReadCommandPType)
	{
	  *str += msg[RawData];
	}
      *str+=':';
      break;
    case (CmdDisplayModeType *256)+CmdDisplayModeSub:
      s+=getversion_rawdata_strings_count;
      *str += rawdata_strings[msg[RawData]+s];
      *str+=':';
      break;
    case (CurrentTestPatternType * 265)+CurrentTestPatternSub:
      s+=displaymode_rawdata_strings_count;
      *str += rawdata_strings[msg[RawData]+s];
      *str+=':';
      break;
    case (CmdLEDCurrentSettingType *256)+CmdLEDCurrentSettingSub:
      s+=testpattern_rawdata_strings_count;
      for(i=0;i<(s+ledcurrentsettings_rawdata_strings_count);i++)
	{
	  *str += rawdata_strings[i];
	  *str += QString("=%1:").arg( msg[(RawData+i)] );
	}
      break;
    case (CmdStaticImageType *256)+CmdStaticImageSub:
      s+=ledcurrentsettings_rawdata_strings_count;
      *str += rawdata_strings[msg[RawData]+s];
      if(msg[PType]==WriteCommandPType)
	{ 
	  *str += ":ADDR=";
	  *str += &msg[RawData];
	}
      *str+=':';
      break;
    case (CmdStaticColorType *256)+CmdStaticColorSub:
      s+=staticimage_rawdata_strings_count;
      *str += rawdata_strings[msg[RawData]+s];
      *str += QString("=%1:").arg(msg[(RawData+i)]);
      break;
    case (CmdDisplayOrientationType *256)+CmdDisplayOrientationSub:
      s+=staticcolor_rawdata_strings_count;
      for(i=0;i<(s+(displayorientation_rawdata_strings_count/2));i++) // three bytes with two choices of 0 or 1
	{
	  *str += rawdata_strings[(i*2)+msg[(RawData+i)]+s];
	  *str+=':';
	}
      break;
    case (CmdVideoInputSettingType *256)+CmdVideoInputSettingSub:
      s+=displayorientation_rawdata_strings_count;
      for(i=0;i<(s+videoinputsetting_rawdata_strings_count);i++)
	{
	  *str += rawdata_strings[i];
	  *str += QString("=%1:").arg(msg[(RawData+i)]);
	}
      break;
    case (CmdVideoModeSettingType *256)+CmdVideoModeSettingSub:
      s+=videoinputsetting_rawdata_strings_count;
      *str += rawdata_strings[msg[RawData]+s];
      *str+=':';
      break;
    default:
      // complain
      *str+="MALFORMED COMMAND";
      break;
    }
}


int CommandParseTest()
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
 
//   char * fake_version_string="Fake version\0";
//   unsigned int addr=(unsigned int)fake_version_string;
//   char *t;
//   t=(char *)&addr;
//   for(int i=0;i<4;i++)
//     msg[RawData+i]=t[i];
//   qDebug() << addr << (char *)msg[RawData];
//   msg[RawData+4]=checksum((char *)msg[RawData],1);
//   t=0;
//   addr=0;
 
}


