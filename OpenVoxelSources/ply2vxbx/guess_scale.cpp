#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QImage>
#include <QColor>
#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include "../../3rdparty/rply-1.1.1/rply.h"
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

  // we must swap all the new params back again IN REVERSE so that when we run ply2vxbx, all is correctly swapped again!!
  if(swapXY)
    {
      std::swap(PLYXCENTER,PLYYCENTER);
      std::swap(XSCL,YSCL);
      std::swap(YLO,XLO);
      std::swap(YHI,XHI);
    }		
  if(swapXZ)
    {
      std::swap(PLYXCENTER,PLYZCENTER);
      std::swap(XSCL,ZSCL);
      std::swap(XLO,ZLO);
      std::swap(XHI,ZHI);
    }		
  if(swapYZ)
    {
      std::swap(PLYYCENTER,PLYZCENTER);
      std::swap(YSCL,ZSCL);
      std::swap(YLO,ZLO);
      std::swap(YHI,ZHI);
    }		

  qDebug() << "Maximums\tXHI " << XHI << " YHI " << YHI << " ZHI " << ZHI;
  qDebug() << "Minimums\tXLO " << XLO << " YLO " << YLO << " ZLO " << ZLO;


  QString dirname( (QFileInfo(ply_file_name).baseName()) );

  QString tmps="ply2vxbx --ply_file ";

  tmps += ply_file_name;
  tmps += QString(" --xscale %1 ").arg(XSCL);
  tmps += QString(" --xoffset %1 ").arg(PLYXCENTER);
  tmps += QString(" --yscale %1 ").arg(YSCL);
  tmps += QString(" --yoffset %1 ").arg(PLYYCENTER);
  tmps += QString(" --zscale %1 ").arg(ZSCL);
  tmps += QString(" --zoffset %1 ").arg(PLYZCENTER);

  if(swapXY)
    tmps+="--swapXY ";
  if(swapXZ)
    tmps+="--swapXZ ";
  if(swapYZ)
    tmps+="--swapYZ ";
  if(fit)
    tmps+="--fit ";
  tmps+="--zoom .80 ";
  tmps+= "2> " + dirname + ".txt\nTwiddle the offset values  to center...\n";
  std::cout << tmps.toStdString().c_str() << std::endl;
  tmps+='\n';
  QString fname=dirname;
  fname+="/";
  fname+=dirname;
  fname+="_guess_args.txt";
  QFile f(fname);
  f.open(QIODevice::WriteOnly);
  f.write(tmps.toStdString().c_str());
  f.close();
  qDebug() << "Wrote " + fname;
  return 0;
}

