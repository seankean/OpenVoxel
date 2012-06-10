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
static int  PLYXCENTER=0;
static int  PLYYCENTER=0;
static int  PLYZCENTER=0;
static bool swapYZ=false;
static bool swapXY=false;
static bool swapXZ=false;
static double t=0;

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

  // we should have eveyone for this line as we have Z called last for each line...
  // scale and add our center to the value...

//   if(swapYZ==true)
//     {
//       t=Y;
//       Y=Z;
//       Z=t;
//       qDebug() << "Swapping Z and Y before scaling";
//       qDebug() << "New Z " << Z << " New Y " << Y;
//     }

//   if(swapXY==true)
//     {
//       t=Y;
//       Y=X;
//       X=t;
//       qDebug() << "Swapping Z and Y before scaling";
//       qDebug() << "New Z " << Z << " New Y " << Y;
//     }


//   int x=0,y=0,z=0;
//   double Xs = ((XSCL*X)+MIDX+PLYXCENTER);
//   double Ys = ((YSCL*Y)+MIDY+PLYYCENTER);
//   double Zs = ((ZSCL*Z)+MIDZ+PLYZCENTER);
//   qDebug() << "After scaling:    \tXs " << Xs << " Ys " << Ys << " Zs " << Zs;

//   x = (int)Xs;
//   y = ( int)Ys;
//   z = ( int)Zs;
  
//   qDebug() << "After assigning to int: x " << x << " y " << y << " z " << z;
  
//   // keep these values in range 1 to (MAX+1)
//   if(  x<1 || y<1 || z<1 || x > (int)(MAXX+1) || y >= (int)(MAXY+1) || z>= (int)(MAXZ+1) )
//     {
//       qDebug() << "Out of Range!!!";
//       return 1;
//     }
//   // now make them all zero based indexes
//   x--;
//   y--;
//   z--;

//   // reverse Z
//   z=71-z;


  
  return 1;
}

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  unsigned int i=0;
  QString ply_file_name="./bunny.ply";
  QStringList arguments = a.arguments();
  
  for(i=1;i <((unsigned int) (arguments.count())); ++i)
    {
      if(arguments.at(i) == "--ply_file")
	ply_file_name = arguments.at(++i);
      
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



  qDebug() << "Maximums\tXHI " << XHI << " YHI " << YHI << " ZHI " << ZHI;
  qDebug() << "Minimums\tXLO " << XLO << " YLO " << YLO << " ZLO " << ZLO;

  qDebug() << "Args: "; 
  QString tmps;
  for(i=0;i<(unsigned int)arguments.count();i++)
    {
      tmps += arguments.at(i);
      tmps += " ";
    }
  qDebug() << tmps;

  QString dirname( (QFileInfo(ply_file_name).baseName()) );
  QString fname=dirname;
  fname+="/";
  fname+=dirname;
  fname+="_args.txt";
  QFile f(fname);
  f.open(QIODevice::WriteOnly);
  f.write(tmps.toStdString().c_str());
  f.close();
  return 0;
}

