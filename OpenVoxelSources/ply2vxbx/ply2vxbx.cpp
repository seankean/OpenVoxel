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
static unsigned int MAXIM=MAXZ/MAXPERLV;
static unsigned int MAXBITPERCOLOR=8;
static double XSCL=0;
static double YSCL=0;
static double ZSCL=0;
static double XLO=0;
static double YLO=0;
static double ZLO=0;
static double XHI=0;
static double YHI=0;
static double ZHI=0;
static bool swapYZ=false;
static bool swapXY=false;
static bool swapXZ=false;
static bool flipX=false;
static bool flipY=false;
static bool flipZ=false;
static bool fit=false;
static double zoom_mid=0;
static double zoom_hi=0;
static double zoom_progressive=0;

QImage imlist[3];
QImage imlist_obverse[3];



static int scale_vertex_cb_x(p_ply_argument argument) {
  long eol;
  ply_get_argument_user_data(argument, NULL, &eol);
  X=ply_get_argument_value(argument);
  return 1;
}

static int scale_vertex_cb_y(p_ply_argument argument) {
  long eol;
  ply_get_argument_user_data(argument, NULL, &eol);
  Y=ply_get_argument_value(argument);
  return 1;
}


static int scale_vertex_cb_z(p_ply_argument argument) {
  long eol;
  ply_get_argument_user_data(argument, NULL, &eol);
  Z=ply_get_argument_value(argument);

  // all loaded - do All the swapping

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

  XHI=std::max(XHI,X);
  XLO=std::min(XLO,X);
  YHI=std::max(YHI,Y);
  YLO=std::min(YLO,Y);
  ZHI=std::max(ZHI,Z);
  ZLO=std::min(ZLO,Z);


  return 1;
}

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
  // we have already swapped HI LO  so just swap the othervalues
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

  int x=0,y=0,z=0;

  //scale
  // qDebug() << "Before scaling: \tX " << X << " Y " << Y << " Z " << Z;
  
  double Xs = XSCL * (X-XLO);
  double Ys = YSCL * (Y-YLO);
  double Zs = ZSCL * (Z-ZLO);

  int tmpz=0;
  if( (zoom_mid != 0.0 && zoom_hi !=0.0 ) && zoom_progressive == 0.0)
    {
      tmpz=(int)Zs;
      if( tmpz>=1 && tmpz<=(int)MAXZ)
	{
	  tmpz--;
	  if(flipZ)
	    tmpz=(MAXZ-1)-tmpz;
	  switch((unsigned int)(z/MAXPERLV))
	    {
	    case 0:
	      // no progressive scaling
	      break;
	    case 1:
	      // apply zoom_mid
	      Xs*=zoom_mid;
	      Ys*=zoom_mid;
	      break;
	    case 2:
	      // apply zoom_hi
	      Xs*=zoom_hi;
	      Ys*=zoom_hi;
	      break;
	    default:
	      break;
	    }
	}
    }
  else if(zoom_progressive != 0.0)
    {
      tmpz=(int)Zs;
      if( tmpz>=1 && tmpz<=(int)MAXZ)
	{
	  tmpz--;
	  if(flipZ)
	    tmpz=(MAXZ-1)-tmpz;

	  Xs*=(zoom_progressive*z);
	  Ys*=(zoom_progressive*z);
	}
    }



  // qDebug() << "After scaling:    \tXs " << Xs << " Ys " << Ys << " Zs " << Zs;
  
  x = (int)Xs;
  y = ( int)Ys;
  z = ( int)Zs;
  
  // qDebug() << "After assigning to int: x " << x << " y " << y << " z " << z;
  
  // keep these values in range 1 to MAX
  if(  x<1 || y<1 || z<1 ||  x > (int)(MAXX) || y > (int)(MAXY) ||  z> (int)(MAXZ) )
    {
      // qDebug() << "Out of Range!!!";
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
  
    
  // qDebug() << "x=" << x << " y=" << y << " z=" << z;
  // qDebug() << " bmp_ind="  << bmp_ind;
  // qDebug() << "adj_z_in_vseg_ind=" << adj_z_in_vseg_ind;
  // qDebug() << "adj_z_in_color_ind=" << adj_z_in_color_ind;
  // qDebug() << "adj_z_bit_pos=" << adj_z_bit_pos ;
  // qDebug() << "c_byte_value_first" << c_byte_value_first;
  // qDebug() << "c_byte_value_second" << c_byte_value_second;

  QPoint ploc( x, y );
  QRgb pcolor=imlist[bmp_ind].pixel(ploc);
  QRgb pcolor_obverse=imlist_obverse[bmp_ind].pixel(ploc);
  int r,g,b;
  int r_o,g_o,b_o;
  
  r=qRed(pcolor);
  g=qGreen(pcolor);
  b=qBlue(pcolor);
  // qDebug() << "Old color(RGB) is " << "[" << r << "]"  << "[" << g << "]"  << "[" << b << "]" ;
  
  r_o=qRed(pcolor_obverse);
  g_o=qGreen(pcolor_obverse);
  b_o=qBlue(pcolor_obverse);
  // qDebug() << "Old color(RGB) Obverse  is " << "[" << r_o << "]"  << "[" << g_o << "]"  << "[" << b_o << "]" ;

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
      // qDebug() << "BAD COLOR IND";
      return 1;
    }

  pcolor=qRgb(r,g,b);
  imlist[bmp_ind].setPixel(QPoint(x,y),pcolor);
  // qDebug() << "New color(RGB) is " << "[" << r << "]"  << "[" << g << "]"  << "[" << b << "]" ;
  pcolor_obverse=qRgb(r_o,g_o,b_o);
  imlist_obverse[bmp_ind].setPixel(QPoint(x,y),pcolor_obverse);
  // qDebug() << "New color(RGB)_obverse is " << "[" << r_o << "]"  << "[" << g_o << "]"  << "[" << b_o << "]" ;

  return 1;
}





int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QImage im(MAXX,MAXY,QImage::Format_RGB32);
  unsigned int i=0;
  QString ply_file_list_name="";
  QString ply_file_name="";
  QStringList arguments = a.arguments();
  
  for(i=1;i <((unsigned int) (arguments.count())); ++i)
    {
      if(arguments.at(i) == "--ply_file_list")
	ply_file_list_name = arguments.at(++i);
      else
	{
	  qDebug() << arguments.at(i) << "-INVALID ARG";
	  qDebug() << "Args:" << arguments;
	  qDebug() << "use --ply_file_list <a file that looks like file_list_test_bunny.txt>";
	  std::exit(1); 
	}
    }
  
  // qDebug() << "Args: "; 
  QString tmps;
  for(i=0;i<(unsigned int)arguments.count();i++)
    {
      tmps += arguments.at(i);
      tmps += " ";
    }

  // qDebug() << tmps;

  // qDebug() <<  "ply_file_list_name" << ply_file_list_name;
  QFile flist;
  flist.setFileName(ply_file_list_name);

  if(!flist.open(QIODevice::ReadOnly))
    {
      qDebug() << "Couldn't open ply_file_list " << ply_file_list_name;
      std::exit(1);
    }
  
  p_ply ply; 
  QImage tmp;
  QString fname,ts;
  QString dirname;
  QStringList line_args;
  QDir qd;
  unsigned int j;
  QFile f;
  char *list_line = new char[1000];
  while(!flist.atEnd())
    {
      XSCL=0;
      YSCL=0;
      ZSCL=0;
      XLO=0;
      YLO=0;
      ZLO=0;
      XHI=0;
      YHI=0;
      ZHI=0;
      swapYZ=false;
      swapXY=false;
      swapXZ=false;
      flipX=false;
      flipY=false;
      flipZ=false;
      fit=false;

      flist.readLine(list_line,1000);
      ts=list_line;
      ts.remove(QRegExp("\n"));
      line_args=QStringList(ts.split(" "));
      // qDebug() << "ts " << ts << "line_args" << line_args << "line args count" << line_args.count();
      for(i=0;i <((unsigned int) (line_args.count()-1)); i++)
	{
	  // qDebug() << "arg: " << line_args.at(i);

	  if(line_args.at(i) == "--ply_file")
	    {
	      // qDebug() << "Got the name"  ;
	      ply_file_name = line_args.at(++i);
	    }
	  else if(line_args.at(i) == "--swapXY")
	    {
	      swapXY=true;
	    }
	  else if(line_args.at(i) == "--swapXZ")
	    {
	      swapXZ=true;
	    }
	  else if(line_args.at(i) == "--swapYZ")
	    {
	      swapYZ=true;
	    }
	  else if(line_args.at(i) == "--flipX")
	    {
	      flipX=true;
	    }
	  else if(line_args.at(i) == "--flipY")
	    {
	      flipY=true;
	    }
	  else if(line_args.at(i) == "--flipZ")
	    {
	      flipZ=true;
	    }
	  else if(line_args.at(i) == "--zoom_mid")
	    {
	      zoom_mid = line_args.at(++i).toDouble();
	    }
	  else if(line_args.at(i) == "--zoom_hi")
	    {
	      zoom_hi = line_args.at(++i).toDouble();
	    }
	  else if(line_args.at(i) == "--zoom_progressive")
	    {
	      zoom_hi = line_args.at(++i).toDouble();
	    }
	  else
	    {
	      qDebug() << line_args.at(i) << "-INVALID ARG LINE ";
	      qDebug() << "Args:" << line_args;;
	      std::exit(1) ;
	    }
	}



      ply = ply_open(ply_file_name.toStdString().c_str(), NULL, 0, NULL);
      if (!ply) 
	{
	   qDebug() << "Couldn't open " << ply_file_name;
	  std::exit(1);
	}
      if (!ply_read_header(ply)) std::exit(1);
  
      ply_set_read_cb(ply, "vertex", "x", scale_vertex_cb_x, NULL, 0);
      ply_set_read_cb(ply, "vertex", "y", scale_vertex_cb_y, NULL, 1);
      ply_set_read_cb(ply, "vertex", "z", scale_vertex_cb_z, NULL, 2);

      if (!ply_read(ply)) 
	std::exit(1);
      ply_close(ply);

      // qDebug() << "After ply read - ";

      // qDebug() << "Maximums\tXHI " << XHI << " YHI " << YHI << " ZHI " << ZHI;
      // qDebug() << "Minimums\tXLO " << XLO << " YLO " << YLO << " ZLO " << ZLO;

      XSCL=((double)(MAXX) / (fabs(XHI)+fabs(XLO)));
      YSCL=((double)(MAXY) / (fabs(YHI)+fabs(YLO)));
      ZSCL=((double)(MAXZ) / (fabs(ZHI)+fabs(ZLO)));

      // qDebug() << "XSCL " << XSCL << "YSCL " << YSCL << "ZSCL " << ZSCL;

      // qDebug() << "Reading data";


      // CREATE THE IMAGES  
  
      for(i=0;i<MAXIM;i++)
	{
	  imlist[i] = im.convertToFormat(QImage::Format_RGB32);
	  imlist_obverse[i] = im.convertToFormat(QImage::Format_RGB32);
	}

      ply = ply_open(ply_file_name.toStdString().c_str(), NULL, 0, NULL);
      if (!ply) std::exit(1);
      if (!ply_read_header(ply)) std::exit(1);
      
      ply_set_read_cb(ply, "vertex", "x", vertex_cb_x, NULL, 0);
      ply_set_read_cb(ply, "vertex", "y", vertex_cb_y, NULL, 1);
      ply_set_read_cb(ply, "vertex", "z", vertex_cb_z, NULL, 2);

      if (!ply_read(ply)) 
	std::exit(1);
      ply_close(ply);

      // qDebug() << "After ply read";
      dirname=QString( (QFileInfo(ply_file_name).baseName()) );
      qd.mkdir(dirname);
      qd.cd(dirname);
      qDebug() << "Creating " << dirname;
      for(j=0;j<MAXIM;j++)
	{
	  fname=dirname;
	  fname+=QString("/image%1.bmp").arg(j, 5, 'f', 0, '0');
	  tmp=imlist[j].convertToFormat(QImage::Format_RGB888);
	  if(!tmp.save(fname,"BMP"))
	    {
	      // qDebug() << "Unable to write " << fname;
	      std::exit(1);
	    }
	  fname=dirname;
	  fname+=QString("/image%1.bmp").arg(6-j, 5, 'f', 0, '0');
	  tmp=imlist_obverse[j].convertToFormat(QImage::Format_RGB888);
	  if(!tmp.save(fname,"BMP"))
	    {
	      // qDebug() << "Unable to write " << fname;
	      std::exit(1);
	    }
	  // qDebug() << "Wrote " << fname;
	}
      // write out blank ones for 3 and 7
 
      fname=dirname;
      fname+=QString("/image%1.bmp").arg(3, 5, 'f', 0, '0');
      tmp=im.convertToFormat(QImage::Format_RGB888);
      if(!tmp.save(fname,"BMP"))
	{
	  // qDebug() << "Unable to write " << fname;
	  std::exit(1);
	}
      // qDebug() << "Wrote " << fname;
      fname=dirname;
      fname+=QString("/image%1.bmp").arg(7, 5, 'f', 0, '0');
      tmp=im.convertToFormat(QImage::Format_RGB888);
      if(!tmp.save(fname,"BMP"))
	{
	  // qDebug() << "Unable to write " << fname;
	  std::exit(1);
	} 
      //qDebug() << "Wrote " << fname;
      // write out a file containing our arguments
      fname=dirname;
      fname+="/";
      fname+=dirname;
      fname+="_ply2vxbx_args.txt";
      f.setFileName(fname);
      f.open(QIODevice::WriteOnly);
      f.write(tmps.toStdString().c_str());
      f.close();
      qd.cd("..");
      qDebug() << "Finished " << dirname;
    }
  
  std::exit(0);
}

