#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QImage>
#include <QColor>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include "../display_tool/3rdparty/rply-1.1.1/rply.h"
#include <math.h>


static double Z=0.0,X=0.0,Y=0.0;
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
static double XSCALE=0;
static double YSCALE=0;
static double ZSCALE=0;
static double XOFFSET=0;
static double YOFFSET=0;
static double ZOFFSET=0;
static bool swapYZ=false;
static bool swapXY=false;
static bool swapXZ=false;
static bool flipX=false;
static bool flipY=false;
static bool flipZ=true;
static double zoom=0.99;
static bool fit=true;
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

	

  // swap

  double TXSCALE=XSCALE;
  double TXOFFSET=XOFFSET;
  double TYSCALE=YSCALE;
  double TYOFFSET=YOFFSET;
  double TZSCALE=ZSCALE;
  double TZOFFSET=ZOFFSET;
  // guess program uses this order so we must as well


  if(swapYZ)
    {
      std::swap(Y,Z);
      std::swap(TYSCALE,TZSCALE);
      std::swap(TYOFFSET,TZOFFSET);
    }
  if(swapXZ)
    {
      std::swap(X,Z);
      std::swap(TXSCALE,TZSCALE);
      std::swap(TXOFFSET,TZOFFSET);
    }
  if(swapXY)
    {
      std::swap(X,Y);
      std::swap(TXSCALE,TYSCALE);
      std::swap(TXOFFSET,TYOFFSET);
    }

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
  y = ( int)Ys; //(fudgefactor)
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

  
  unsigned int bmp_ind=(unsigned int)(z/MAXPERLV);  
  unsigned int adj_z_in_vseg_ind=(unsigned int)(z-(bmp_ind*MAXPERLV)); 
  unsigned int adj_z_in_color_ind=(unsigned int)(adj_z_in_vseg_ind/MAXBITPERCOLOR); 
  unsigned int adj_z_bit_pos=(unsigned int)adj_z_in_vseg_ind-(adj_z_in_color_ind*MAXBITPERCOLOR);
  unsigned int c_byte_value_first = (unsigned int)pow(2,(adj_z_bit_pos)) ;
  unsigned int c_byte_value_second = (unsigned int)pow(2,(MAXBITPERCOLOR-(adj_z_bit_pos))); // the first bit[7] becomes bit[1] the second time.  
    
    
  qDebug() << "x=" << x << " y=" << y << " z=" << z;
  qDebug() << " bmp_ind="  << bmp_ind;
  qDebug() << "adj_z_in_vseg_ind=" << adj_z_in_vseg_ind;
  qDebug() << "adj_z_in_color_ind=" << adj_z_in_color_ind;
  qDebug() << "adj_z_bit_pos=" << adj_z_bit_pos ;
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
  qDebug() << "Old color(RGB) is " << "[" << r << "]"  << "[" << g << "]"  << "[" << b << "]" ;
  
  r_o=qRed(pcolor_obverse);
  g_o=qGreen(pcolor_obverse);
  b_o=qBlue(pcolor_obverse);
  qDebug() << "Old color(RGB) Obverse  is " << "[" << r_o << "]"  << "[" << g_o << "]"  << "[" << b_o << "]" ;

  switch(adj_z_in_color_ind)
    {
    case 0: // B up G down
      b |= c_byte_value_first;
      g_o |= c_byte_value_second;
      break;
    case 1: // R up R down
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
  QImage im(MAXX,MAXY,QImage::Format_RGB32);
  unsigned int i=0;
  QString ply_file_name="";
  QStringList arguments = a.arguments();
  double ply_xscale=0;
  double ply_yscale=0;
  double ply_zscale=0;
  double xoffset=0;
  double yoffset=0;
  double zoffset=0;
 
  for(i=1;i <((unsigned int) (arguments.count())); ++i)
    {
      if(arguments.at(i) == "--ply_file")
	ply_file_name = arguments.at(++i);
      else if(arguments.at(i) == "--xscale")
	{
	  ply_xscale = arguments.at(++i).toDouble();
	}
      else if(arguments.at(i) == "--yscale")
	{
	  ply_yscale = arguments.at(++i).toDouble();
	}
      else if(arguments.at(i) == "--zscale")
	{
	  ply_zscale = arguments.at(++i).toDouble();
	}
      else if(arguments.at(i) == "--xoffset")
	{
	  xoffset = arguments.at(++i).toDouble();
	}
      else if(arguments.at(i) == "--yoffset")
	{
	  yoffset = arguments.at(++i).toDouble();
	}
      else if(arguments.at(i) == "--zoffset")
	{
	  zoffset = arguments.at(++i).toDouble();
	}
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
      else if(arguments.at(i) == "--flipX")
	{
	  flipX=true;
	}
      else if(arguments.at(i) == "--flipY")
	{
	  flipY=true;
	}
      else if(arguments.at(i) == "--flipZ")
	{
	  flipZ=true;
	}
      else if(arguments.at(i) == "--fit")
	{
	  fit=true;
	  MAXY=MAXX;
	}
      else if(arguments.at(i) == "--zoom")
	{
	  zoom = arguments.at(++i).toDouble();
	}
      else
	{
	  qDebug() << arguments.at(i) << "-INVALID ARG";
	  qDebug() << "Args:" << arguments;;
	  return(1); 
	}
    }
  qDebug() << "Args: "; 
  QString tmps;
  for(i=0;i<(unsigned int)arguments.count();i++)
    {
      tmps += arguments.at(i);
      tmps += " ";
    }
  qDebug() << tmps;
  XSCALE=ply_xscale;
  YSCALE=ply_yscale;
  ZSCALE=ply_zscale;
  XOFFSET=xoffset;
  YOFFSET=yoffset;
  ZOFFSET=zoffset;

  
  if(swapYZ)
    {
      qDebug() << "Swapping Y and Z before scaling";
      std::swap(flipY,flipZ);
    }
  if(swapXZ)
    {
      qDebug() << "Swapping X and Z before scaling";
      std::swap(flipX,flipZ);
    }
  if(swapXY)
    {
      qDebug() << "Swapping X and Y before scaling";
      std::swap(flipY,flipY);
    }
  
  
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
  // write out a file containing our arguments
  fname=dirname;
  fname+="/";
  fname+=dirname;
  fname+="_ply2vxbx_args.txt";
  QFile f(fname);
  f.open(QIODevice::WriteOnly);
  f.write(tmps.toStdString().c_str());
  f.close();
  return 0;
}

