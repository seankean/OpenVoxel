#include <QFile>
#include <QDebug>
#include <QImage>
#include <QColor>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include "../display_tool/3rdparty/rply-1.1.1/rply.h"
#include <math.h>
#include <iostream>

double Z=0.0,X=0.0,Y=0.0;
static  int MAXX=608;
static unsigned int MAXY=684;
static unsigned int MAXZ = 72;
static unsigned int MAXB=MAXX*MAXY;
static unsigned int MIDX=MAXX/2;
static unsigned int MIDY=MAXY/2;
static unsigned int MIDZ=MAXZ/2;
static double XSCL=0.0;
static double YSCL=0.0;
static double ZSCL=0.0;
static double XHI=0.0;
static double YHI=0.0;
static double ZHI=0.0;
static double XLO=0.0;
static double YLO=0.0;
static double ZLO=0.0;
static double PLYXCENTER=0;
static double PLYYCENTER=0;
static double PLYZCENTER=0;
static bool swapYZ=false;
static bool swapXY=false;
static bool swapXZ=false;
static bool fit=true;

static int vertex_cb_x(p_ply_argument argument) {
  long eol;
  ply_get_argument_user_data(argument, NULL, &eol);
  X=ply_get_argument_value(argument);
  XHI=std::max(XHI,X);
  XLO=std::min(XLO,X);
  return 1;
}

static int vertex_cb_y(p_ply_argument argument) {
  long eol;
  ply_get_argument_user_data(argument, NULL, &eol);
  Y=ply_get_argument_value(argument);
  YHI=std::max(YHI,Y);
  YLO=std::min(YLO,Y);
  return 1;
}


static int vertex_cb_z(p_ply_argument argument) {
  long eol;
  ply_get_argument_user_data(argument, NULL, &eol);
  Z=ply_get_argument_value(argument);
  ZHI=std::max(ZHI,Z);
  ZLO=std::min(ZLO,Z);

  // all loaded - do the swapping

  if(swapYZ)
    {
      std::swap(Y,Z);
    }
  if(swapXZ)
    {
      std::swap(X,Z);
    }
  if(swapXY)
    {
      std::swap(Y,X);
    }
  return 1;
}

int guess(int argc, char *argv[])
{

  unsigned int i=0;
  QString ply_file_name="./bunny.ply";
  QStringList arguments = (QString(argv).split());
  
  for(i=1;i <((unsigned int) (arguments.count())); ++i)
    {
      if(arguments.at(i) == "--ply_file")
	ply_file_name = arguments.at(++i);
      else if(arguments.at(i) == "--swapXY")
	{
	  swapXY=true;
	}
      else if(arguments.at(i) == "--swapXZ")
	{
	  swapXZ=true;
	}
      else if(arguments.at(i) == "--swapYZ")
	{
	  swapYZ=true;
	}
      else if(arguments.at(i) == "--noswapXY")
	{
	  swapXY=false;
	}
      else if(arguments.at(i) == "--noswapXZ")
	{
	  swapXZ=false;
	}
      else if(arguments.at(i) == "--noswapYZ")
	{
	  swapYZ=false;
	}
      else if(arguments.at(i) == "--fit")
	{
	  fit=true;
	  MAXY=MAXX;
	}
      else
	{
	  qDebug() << arguments.at(i) << "-INVALID ARG";
	  qDebug() << "Args:" << arguments;;
	  return(1); 
	}
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
 
  if(swapYZ)
    {
      std::swap(YLO,ZLO);
      std::swap(YHI,ZHI);
    }
  if(swapXZ)
    {
      std::swap(XLO,ZLO);
      std::swap(XHI,ZHI);
    }
  if(swapXY)
    {
      std::swap(YLO,XLO);
      std::swap(YHI,XHI);
    }


  XSCL=(double)(MAXX-1) / (fabs(XHI)+fabs(XLO));
  YSCL=(double)(MAXY-1) / (fabs(YHI)+fabs(YLO));
  ZSCL=(double)(MAXZ-1) / (fabs(ZHI)+fabs(ZLO));
  PLYXCENTER=1+-(XSCL*XLO);
  PLYYCENTER=1+-(YSCL*YLO);
  PLYZCENTER=1+-(ZSCL*ZLO);


  qDebug() << "Maximums\tXHI " << XHI << " YHI " << YHI << " ZHI " << ZHI;
  qDebug() << "Minimums\tXLO " << XLO << " YLO " << YLO << " ZLO " << ZLO;

  return 0;
}


// QImage imlist[3];
// QImage imlist_obverse[3];

// unsigned int **list;

static int vertex_cb_x2(p_ply_argument argument) {
  long eol;
  ply_get_argument_user_data(argument, NULL, &eol);
  X=ply_get_argument_value(argument);
  
  return 1;
}

static int vertex_cb_y2(p_ply_argument argument) {
  long eol;
  ply_get_argument_user_data(argument, NULL, &eol);
  Y=ply_get_argument_value(argument);
  return 1;
}


static int vertex_cb_z2(p_ply_argument argument) {
  long eol;
  ply_get_argument_user_data(argument, NULL, &eol);
  Z=ply_get_argument_value(argument);

  // we should have eveyone for this line as we have Z called last for each line...


  double TXSCALE=XSCALE;
  double TXOFFSET=XOFFSET;
  double TYSCALE=YSCALE;
  double TYOFFSET=YOFFSET;
  double TZSCALE=ZSCALE;
  double TZOFFSET=ZOFFSET;

  //scale
  qDebug() << "Before scaling: \tX " << X << " Y " << Y << " Z " << Z;
 
  int x=0,y=0,z=0;
  double Xs = ((TXSCALE* X))*zoom;
  double Ys = ((TYSCALE * Y))*zoom;
  double Zs = ((TZSCALE* Z))*zoom;

  qDebug() << "After scaling:    \tXs " << Xs << " Ys " << Ys << " Zs " << Zs;

  Xs+=(TXOFFSET*zoom);
  Ys+=(TYOFFSET*zoom);
  Zs+=(TZOFFSET*zoom);

  x = (int)Xs;
  y = ( int)Ys;
  z = ( int)Zs;

  qDebug() << "After assigning to int: x " << x << " y " << y << " z " << z;
  
  // keep these values in range 1 to (MAX+1)
  if(  x<1 || y<1 || z<1 || x > (int)(MAXX+1) || y > (int)(MAXY+1) || z> (int)(MAXZ+1) )
    {
      qDebug() << "Out of Range!!!";
      return 1;
    }
  // now make them all zero based indexes
  x--;
  y--;
  z--;

  // reverse Z
  if(flipX)
    x=(MAXX-1)-x;
  if(flipY)
    y=(MAXY-1)-y;
  if(flipZ)
    z=(MAXZ-1)-z;

  buf[pos+=4][;
  // put them into the buffer

//   unsigned int bmp_ind=(unsigned int)(z/MAXPERLV);  
//   unsigned int adj_z_in_vseg_ind=(unsigned int)(z-(bmp_ind*MAXPERLV)); 
//   unsigned int adj_z_in_color_ind=(unsigned int)(adj_z_in_vseg_ind/MAXBITPERCOLOR); 
//   unsigned int adj_z_bit_pos=(unsigned int)adj_z_in_vseg_ind-(adj_z_in_color_ind*MAXBITPERCOLOR);
//   unsigned int c_byte_value_first = (unsigned int)pow(2,(adj_z_bit_pos)) ;
//   unsigned int c_byte_value_second = (unsigned int)pow(2,(MAXBITPERCOLOR-(adj_z_bit_pos))); // the first bit[7] becomes bit[1] the second time.  
    
    
//   qDebug() << "x=" << x << " y=" << y << " z=" << z;
//   qDebug() << " bmp_ind="  << bmp_ind;
//   qDebug() << "adj_z_in_vseg_ind=" << adj_z_in_vseg_ind;
//   qDebug() << "adj_z_in_color_ind=" << adj_z_in_color_ind;
//   qDebug() << "adj_z_bit_pos=" << adj_z_bit_pos ;
//   qDebug() << "c_byte_value_first" << c_byte_value_first;
//   qDebug() << "c_byte_value_second" << c_byte_value_second;

//   QPoint ploc( x, y );
//   QRgb pcolor=imlist[bmp_ind].pixel(ploc);
//   QRgb pcolor_obverse=imlist_obverse[bmp_ind].pixel(ploc);
//   int r,g,b;
//   int r_o,g_o,b_o;
  
//   r=qRed(pcolor);
//   g=qGreen(pcolor);
//   b=qBlue(pcolor);
//   qDebug() << "Old color(RGB) is " << "[" << r << "]"  << "[" << g << "]"  << "[" << b << "]" ;
  
//   r_o=qRed(pcolor_obverse);
//   g_o=qGreen(pcolor_obverse);
//   b_o=qBlue(pcolor_obverse);
//   qDebug() << "Old color(RGB) Obverse  is " << "[" << r_o << "]"  << "[" << g_o << "]"  << "[" << b_o << "]" ;

//   switch(adj_z_in_color_ind)
//     {
//     case 0: // B up G down
//       b |= c_byte_value_first;
//       g_o |= c_byte_value_second;
//       break;
//     case 1: // R up R down
//       r |= c_byte_value_first;
//       r_o |= c_byte_value_second;
//       break;
//     case 2: // G 
//       g |= c_byte_value_first;
//       g_o |= c_byte_value_second;
//       break;
//     default:
//       qDebug() << "BAD COLOR IND";
//       return 1;
//     }

//   pcolor=qRgb(r,g,b);
//   imlist[bmp_ind].setPixel(QPoint(x,y),pcolor);
//   qDebug() << "New color(RGB) is " << "[" << r << "]"  << "[" << g << "]"  << "[" << b << "]" ;
//   pcolor_obverse=qRgb(r_o,g_o,b_o);
//   imlist_obverse[bmp_ind].setPixel(QPoint(x,y),pcolor_obverse);
//   qDebug() << "New color(RGB)_obverse is " << "[" << r_o << "]"  << "[" << g_o << "]"  << "[" << b_o << "]" ;

  return 1;
}

int ply2buf(char *buf)
{
  // buffer is MAXX*MAXY*MAXZ*sizeof(int)  
  char *buf=new int[MAXY*3];
  p_ply ply = ply_open(ply_file_name.toStdString().c_str(), NULL, 0, NULL);
  if (!ply) return 1;
  if (!ply_read_header(ply)) return 1;
  
  ply_set_read_cb(ply, "vertex", "x", vertex_cb_x2, NULL, 0);
  ply_set_read_cb(ply, "vertex", "y", vertex_cb_y2, NULL, 1);
  ply_set_read_cb(ply, "vertex", "z", vertex_cb_z2, NULL, 2);
  if (!ply_read(ply)) 
    return 1;
  ply_close(ply);
  qDebug() << "After ply read";
  return 0;
}

