#include "config.h" // Xplatform specific includes should go in config.h 
#include <QtGui/QApplication>
#include <QDebug>
#include "widget.h"
#include "qextserialport.h"
#include "PortListener.h"
#include <iostream>

QString serial_device = "COM3";
QString camera_ip = "192.168.1.100:55555"; //????
QString image_prefix = "testimages/image";
int delay_before = 0;
int delay_after = 0;
int num_images = 1;
int max_time = 1;
bool interlace_levels = false;
bool fullscreen=false;
bool numbers=false;

void dump_args()
{
	qDebug() << "Args:";
	qDebug() << "serial_device " <<  serial_device;
	qDebug() << "camera_ip " <<  camera_ip;
	qDebug() << "image_prefix " << image_prefix;
	qDebug() << "delay_before " << delay_before;
	qDebug() << "delay_after " << delay_after;
	qDebug() << "max_time " << max_time;
	qDebug() << "num_images " << num_images;
	qDebug() << "interlace_levels " << interlace_levels;
	qDebug() << "fullscreen " << fullscreen;
	qDebug() << "numbers " << fullscreen;
	
	//  qDebug() << "colors " << fullscreen;
}

void usage()
{
	std::cerr 
		<< "DisplayProgram:" << std::endl
		<< "The current version of DisplayProgram will connect to the named serial device" << std::endl
		<< "sending it the character 'A'/'B'(up/down).  The images are then rendered to display, " << std::endl  
		<< "starting at image 0 when direction is up and conversly at (num_images-1) when the direction is down." << std::endl
		<< "This continues until 'ESC' is pressed or the process killed." << std::endl
		<< std::endl <<  "Images are assumed to be named <prefix><xxxxx>.bmp.  Prefix can include the path" << std::endl
		<< "e.g. --image_prefix tarzanimages/image would try to load files in the form of tarzanimages/image00000.bmp" << std::endl
		<< "Windows paths should be double backslashes - tarzanimages\\image" << std::endl
		<< "Usage: " << std::endl
		<< "--num_images \t<number of image files to read> Required" << std::endl
		<< "--image_prefix \t<path(s) and alpha filename prefix> (Default is testimages/image)" << std::endl
		<< "--fullscreen \t Enables FullScreen at startup " << std::endl
		<< "--numbers \t Enables layer numbers at startup " << std::endl
		<< "--interlace_levels \t Enables interlaced levels drawing every-other frame as follows: " << std::endl
		<< "\t\t using num_images of 8 would first draw [0,2,4,6] on UP and [7,5,3,1] on DOWN.  (Default is NON-INTERLACED)" << std::endl
		<< "--serial_device \t<name of serial port> (Default is COM3) " << std::endl
		<< "--camera_ip \t<ipaddr:port of the camera> NOT_IMPLEMENTED " << std::endl
		<< "--delay_before \t<msecond delay before images are rendered> (Default is 0) " << std::endl
		<< "--delay_after \t<msecond delay after images are rendered>" << std::endl
		<< "--max_time \t<time in seconds to run>" << std::endl;
}



int main(int argc, char *argv[])
{
	QApplication a(argc, argv);


	// Parse and process arguments


	QStringList arguments = a.arguments();

	for(int i=1; i<arguments.count(); ++i)
	{
		if(arguments.at(i) == "--serial_device")
		{
			serial_device = arguments.at(++i);
		}
		else if(arguments.at(i) == "--camera_ip")
		{
			camera_ip = arguments.at(++i);
		}
		else if(arguments.at(i) == "--interlace_levels")
		{
			interlace_levels=true;
		}
		else if(arguments.at(i) == "--fullscreen")
		{
			fullscreen=true;
		}
		else if(arguments.at(i) == "--max_time")
		{
			max_time = arguments.at(++i).toInt();
		}
		else if(arguments.at(i) == "--numbers")
		{
			numbers=true;
		}
		else if(arguments.at(i) == "--image_prefix")
		{
			image_prefix = arguments.at(++i);
		}
		else if(arguments.at(i) == "--delay_before")
		{
			delay_before = arguments.at(++i).toInt();
		}
		else if(arguments.at(i) == "--delay_after")
		{
			delay_after = arguments.at(++i).toInt();
		}
		else if(arguments.at(i) == "--num_images")
		{
			num_images = arguments.at(++i).toInt();
			if(!num_images)
			{
				std::cerr << std::endl << std::endl << "DisplayProgram - ERROR!  --num_images must be > 0" << std::endl;\
					dump_args();
				usage();
				exit(1);
			}
		}
		else 
		{
			qDebug() << arguments.at(i) << "-INVALID ARG";
			dump_args();
			usage();
			exit(1);
		}
	}
	dump_args();
	QGL::setPreferredPaintEngine(QPaintEngine::OpenGL);
	QGLFormat *format = new QGLFormat(QGL::SampleBuffers);
	format->setSwapInterval(1); // vsync
	Widget w(delay_before, delay_after, max_time, num_images,&serial_device,&camera_ip,&image_prefix,interlace_levels,numbers,0,format);
	w.resize(608,684);
	if(fullscreen)
		w.showFullScreen();
	else
		w.show();


	return a.exec();
}
