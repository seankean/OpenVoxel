#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QImage>
#include <QColor>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include "../DisplayProgram/3rdparty/rply-1.1.1/rply.h"
#include <math.h>
double Z=0.0,X=0.0,Y=0.0;

static unsigned int MAXX=608;
static unsigned int MAXY=684;
static unsigned int MAXZ = 72;
static unsigned int MAXPERLV = 24;
static unsigned int MAXB=MAXX*MAXY;
static unsigned int MIDX=MAXX/2;
static unsigned int MIDY=MAXY/2;
static unsigned int MIDZ=MAXZ/2;
static unsigned int MAXIM=MAXZ/MAXPERLV;
static unsigned int MAXBITPERCOLOR=8;
static int XSCL=1000;
static int YSCL=1000;
static int ZSCL=1000;
static int  PLYXCENTER=0;
static int  PLYYCENTER=0;
static int  PLYZCENTER=0;


QImage imlist[3];
QImage imlist_obverse[3];

unsigned int **list;

static int vertex_cb_x(p_ply_argument argument) {
  long eol;
  ply_get_argument_user_data(argument, NULL, &eol);
  X=ply_get_argument_value(argument);
  
  return 1;
}

static int vertex_cb_y(p_ply_argument argument) {
  long eol;
  ply_get_argument_user_data(argument, NULL, &eol);
  Y=ply_get_argument_value(argument);
  return 1;
}


static int vertex_cb_z(p_ply_argument argument) {
  long eol;
  ply_get_argument_user_data(argument, NULL, &eol);
  Z=ply_get_argument_value(argument);

  // we should have eveyone for this line as we have Z called last for each line...
  // scale and add our center to the value...

  unsigned int x=0,y=0,z=0;
  x = (XSCL*X)+MIDX+PLYXCENTER;
  y = (YSCL*Y)+MIDY+PLYYCENTER;
  z = (ZSCL*Z)+MIDZ+PLYZCENTER;
  

  if( x<0 || x >= (int)MAXX || y<0 || y>= (int)MAXY || z<=0 || z>= (int)MAXZ )
    return 1;

  // z is 1-72 lets make it an index [0-71]
  z--;
  // Example for z=40(41):
  // bmp_ind=(unsigned int)(z/MAXPERLV):bmp_ind=(int)(40/MAXPERLV) = 1.66 = 1 which is the middle image set.
  // adj_z_in_vseg_ind=(unsigned int)(z-(bmp_ind*MAXPERLV)): (unsigned int)(40-(1*24)) = 16 which is the (top,last,pos) for R
  // adj_z_in_color_ind=(unsigned int)(MAXPERLVL/adj_z_in_vseg_ind): (24/16) = 1.5 = 1 [0,1,2] = Red
  // adj_z__bit_val=(unsigned int)adj_z_in_vseg_ind-(int(adj_z_in_color_ind*MAXBITPERCOLOR)): 16-(int(1*8)) = 8 = bit [7](8)
  // the bit position is position 8[7] aand the color is red and the image is the middle.
  // first image paint is the byte-compliment of the value/vs the index.  Second image paint is the normal byte value.

  // c_byte_value_first = pow(2,(adj_z__bit_val-1)) 
  // c_byte_value_second = pow(2,(8-(adj_z__bit_val-1))) the first bit[7] becomes bit[1] the second time.  

  unsigned int bmp_ind=(unsigned int)(z/MAXPERLV); 
  unsigned int adj_z_in_vseg_ind=(unsigned int)(z-(bmp_ind*MAXPERLV)); 
  unsigned int adj_z_in_color_ind=(unsigned int)(adj_z_in_vseg_ind/MAXBITPERCOLOR); 
  unsigned int adj_z_bit_val=(unsigned int)adj_z_in_vseg_ind-(int(adj_z_in_color_ind*MAXBITPERCOLOR));
  int c_byte_value_first = pow(2,(adj_z_bit_val-1)) ;
  int c_byte_value_second = pow(2,(8-(adj_z_bit_val-1))); // the first bit[7] becomes bit[1] the second time.  
    
    
  qDebug() << "x=" << x << " y=" << y << " z=" << z;
  qDebug() << " bmp_ind="  << bmp_ind;
  qDebug() << "adj_z_in_vseg_ind=" << adj_z_in_vseg_ind;
  qDebug() << "adj_z_in_color_ind=" << adj_z_in_color_ind;
  qDebug() << "adj_z_bit_pos=" << adj_z_bit_val-1 ;
  qDebug() << "c_byte_value_first" << c_byte_value_first;
  qDebug() << "c_byte_value_second" << c_byte_value_second;

  QPoint ploc( x, y );
  QRgb pcolor=imlist[bmp_ind].pixel(ploc);
  QRgb pcolor_obverse=imlist_obverse[bmp_ind].pixel(ploc);
  int r,g,b;
  int r_o,g_o,b_o;
  
  r=qRed(pcolor);
  g=qGreen(pcolor);
  b=qBlue(pcolor);
  qDebug() << "Old color(RGB) is " << "[" << QString::number(r,2) << "]"  << "[" << g << "]"  << "[" << b << "]" ;
  
  r_o=qRed(pcolor_obverse);
  g_o=qGreen(pcolor_obverse);
  b_o=qBlue(pcolor_obverse);
  qDebug() << "Old color(RGB) Obverse  is " << "[" << r_o << "]"  << "[" << g_o << "]"  << "[" << b_o << "]" ;

  switch(adj_z_in_color_ind)
    {
    case 0: // B
      b |= c_byte_value_first;
      b_o |= c_byte_value_second;
      break;
    case 1: // R
      r |= c_byte_value_first;
      r_o |= c_byte_value_second;
      break;
    case 2: // G
      g |= c_byte_value_first;
      g_o |= c_byte_value_second;
      break;
    default:
      qDebug() << "BAD COLOR IND";
      return 1;
    }

  pcolor=qRgb(r,g,b);
  imlist[bmp_ind].setPixel(QPoint(x,y),pcolor);
  qDebug() << "New color(RGB) is " << "[" << r << "]"  << "[" << g << "]"  << "[" << b << "]" ;
  pcolor_obverse=qRgb(r_o,g_o,b_o);
  imlist_obverse[bmp_ind].setPixel(QPoint(x,y),pcolor_obverse);
  qDebug() << "New color(RGB)_obverse is " << "[" << r_o << "]"  << "[" << g_o << "]"  << "[" << b_o << "]" ;

  return 1;
}

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QImage im(608,684,QImage::Format_RGB32);
  unsigned int i=0;
  QString ply_file_name="./bunny.ply";
  QStringList arguments = a.arguments();
  int ply_xscale=1000;
  int ply_yscale=1000;
  int ply_zscale=1000;
  int ply_xcenter=0;
  int ply_ycenter=0;
  int ply_zcenter=0;
 
  for(i=1;i <((unsigned int) (arguments.count())); ++i)
    {
      if(arguments.at(i) == "--ply_file")
	ply_file_name = arguments.at(++i);
      else if(arguments.at(i) == "--xscale")
	{
	  ply_xscale = arguments.at(++i).toInt();
	}
      else if(arguments.at(i) == "--yscale")
	{
	  ply_yscale = arguments.at(++i).toInt();
	}
      else if(arguments.at(i) == "--zscale")
	{
	  ply_zscale = arguments.at(++i).toInt();
	}
      else if(arguments.at(i) == "--ply_xcenter")
	{
	  ply_xcenter = arguments.at(++i).toInt();
	}
      else if(arguments.at(i) == "--ply_ycenter")
	{
	  ply_ycenter = arguments.at(++i).toInt();
	}
      else if(arguments.at(i) == "--ply_zcenter")
	{
	  ply_zcenter = arguments.at(++i).toInt();
	}
      else
	{
	  qDebug() << arguments.at(i) << "-INVALID ARG";
	  qDebug() << "Args:" << arguments;;
	  return(1); 
	}
    }
  qDebug() << "Args:";
  qDebug() << "ply_file " <<  ply_file_name;
  qDebug() << "ply_xscale " <<  ply_xscale;
  qDebug() << "ply_yscale " <<  ply_yscale;
  qDebug() << "ply_zscale " <<  ply_zscale;
  qDebug() << "ply_xcenter " <<  ply_xcenter;
  qDebug() << "ply_ycenter " <<  ply_ycenter;
  qDebug() << "ply_zcenter " <<  ply_zcenter;
  
    
  XSCL=ply_xscale;
  YSCL=ply_yscale;
  ZSCL=ply_zscale;
  PLYXCENTER=ply_xcenter;
  PLYYCENTER=ply_ycenter;
  PLYZCENTER=ply_zcenter;
  qDebug() << "MAXX=" << MAXX;
  qDebug() << "MAXY=" << MAXY;
  qDebug() << "MAXZ=" << MAXZ ;
  qDebug() << "MAXPERLV=" << MAXPERLV ;
  qDebug() << "MAXB=" << MAXB;
  qDebug() << "MIDX=" << MIDX;
  qDebug() << "MIDY=" << MIDY;
  qDebug() << "MIDZ=" << MIDZ;
  qDebug() << "MAXIM=" << MAXIM;
  qDebug() << "XSCL=" << XSCL;
  qDebug() << "YSCL=" << YSCL;
  qDebug() << "ZSCL=" << ZSCL;
  qDebug() << "PLYXCENTER" << PLYXCENTER;
  qDebug() << "PLYYCENTER" << PLYYCENTER;
  qDebug() << "PLYZCENTER" << PLYZCENTER;
  PLYYCENTER=ply_ycenter;
  PLYZCENTER=ply_zcenter;

  for(i=0;i<MAXIM;i++)
    {
      imlist[i] = im.convertToFormat(QImage::Format_RGB32);
      imlist_obverse[i] = im.convertToFormat(QImage::Format_RGB32);
    }
  p_ply ply = ply_open(ply_file_name.toStdString().c_str(), NULL, 0, NULL);
  if (!ply) return 1;
  if (!ply_read_header(ply)) return 1;
  
  ply_set_read_cb(ply, "vertex", "x", vertex_cb_x, NULL, 0);
  ply_set_read_cb(ply, "vertex", "y", vertex_cb_y, NULL, 1);
  ply_set_read_cb(ply, "vertex", "z", vertex_cb_z, NULL, 2);
  if (!ply_read(ply)) 
    return 1;
  ply_close(ply);
  qDebug() << "After ply read";
  QImage tmp;
  QString fname;
  QString dirname( (QFileInfo(ply_file_name).baseName()) );
   QDir qd;
  qd.mkdir(dirname);
  qd.cd(dirname);
  
  for(unsigned int j=0;j<MAXIM;j++)
    {
      fname=dirname;
      fname+=QString("/image%1.bmp").arg(j, 5, 'f', 0, '0');
      tmp=imlist[j].convertToFormat(QImage::Format_RGB888);
      if(!tmp.save(fname,"BMP"))
	{
	  qDebug() << "Unable to write " << fname;
	  return 1;
	}
      fname=dirname;
      fname+=QString("/image%1.bmp").arg(6-j, 5, 'f', 0, '0');
      tmp=imlist_obverse[j].convertToFormat(QImage::Format_RGB888);
      if(!tmp.save(fname,"BMP"))
	{
	  qDebug() << "Unable to write " << fname;
	  return 1;
	}
      qDebug() << "Wrote " << fname;
    }
  // write out blank ones for 3 and 7
 
  fname=dirname;
  fname+=QString("/image%1.bmp").arg(3, 5, 'f', 0, '0');
  tmp=im.convertToFormat(QImage::Format_RGB888);
  if(!tmp.save(fname,"BMP"))
    {
      qDebug() << "Unable to write " << fname;
      return 1;
    }
  qDebug() << "Wrote " << fname;
  fname=dirname;
  fname+=QString("/image%1.bmp").arg(7, 5, 'f', 0, '0');
  tmp=im.convertToFormat(QImage::Format_RGB888);
  if(!tmp.save(fname,"BMP"))
    {
      qDebug() << "Unable to write " << fname;
      return 1;
    } 
  qDebug() << "Wrote " << fname;
  
  return 0;
}

