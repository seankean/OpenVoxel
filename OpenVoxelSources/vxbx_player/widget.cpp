#include "widget.h"
//#include "helpers.h"
#include <QThread>
#include <QTimer>
#include <QDebug>
#include <QShortcut>
#include <QtGui>


#include "qextserialport.h" // serial for arduino
#include "PortListener.h" // serial for arduino
#include "config.h"
class Helper: public QThread 
{
public:
  static void msleep(int ms)
  {
    QThread::msleep(ms);
  }
};


Widget::Widget(int delay_before, int delay_after, int max_time, int num_images, QString *serial_device, QString *camera_ip, QString *image_prefix, bool interlace_levels, bool numbers, QWidget *parent, QGLFormat *format, bool rotate, QString *file_list_name)
  :QGLWidget(*format, parent)
  , m_current(0)
  , m_state(Stopped)
  , m_delay_before(delay_before)
  , m_delay_after(delay_after)
  , m_max_time(max_time)
  , m_num_images(num_images)
  , m_port_value(0x0)
  , m_first_run(true)
  , m_serial_device(serial_device)
  , m_camera_ip(camera_ip)
  , m_image_prefix(image_prefix)
  , m_interlace_levels(interlace_levels)
  , m_numbers(numbers)
  , m_rotate(rotate)
  , m_file_list_name(file_list_name)
{
  setAutoFillBackground(false);
  makeCurrent();
  
  // Escape shortcut
  QShortcut *sh = new QShortcut(QKeySequence(trUtf8("Esc")), this);
  connect(sh, SIGNAL(activated()), this, SLOT(close()));
  m_total=0;

  // Info0
  QMessageBox::information(this, tr("Info"),tr("HIT 'ESC' to CLOSE"));  
 
  QString portName(*m_serial_device);           
  listener = new PortListener(portName);        // signals get hooked up internally


    m_direction=1;

  // Images
   for( int i=0; i<m_num_images; i++)
     {
       QPixmap p;
       QString fname=*m_image_prefix;
       fname+=QString("%1.bmp").arg(i, 5, 'f', 0, '0');
       bool ret = p.load(fname);
       if(ret)
    m_pixmap << p;
       else
    m_pixmap << QPixmap();  // empty in case of missing images
   }

//       qDebug() << "Loading image:" << i << "=" << ret;
//     }

  // create num_images empty images...

  m_curr_angle = 0;
  m_draw_count = 0;
  //  m_timer = new QTimer(this);
  m_timespent= new QElapsedTimer();
  m_timelastdraw= new QElapsedTimer();
  //  connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
  //  m_timer->setInterval(50); // times a second we paint drawmax frames 
  
  // if list then open it

 file_list=false;


  if(*m_file_list_name!="")
    {
      flist= new QFile();
      file_list=true;
      flist->setFileName(*file_list_name);
    if(!flist->open(QIODevice::ReadOnly))
	{
	  qDebug() << "Couldn't open file_list " << *file_list_name;
	  std::exit(1);
	}
      //open 
    }
	
	QTimer::singleShot(0, this, SLOT(startShow()));
}

Widget::~Widget()
{
  m_serial_device=0;
  m_camera_ip=0;
  m_image_prefix=0;
  if (listener)
    {
      delete listener;
      listener=0;
    }
}


void Widget::paintEvent(QPaintEvent *)
{
  int drawmax =  m_max_time * 60;
  QString c;
  QPainter painter(this);
  QThread t(this);

  QElapsedTimer  loopTime; //time between frames
  QElapsedTimer  drawTime; //time between frames

  loopTime.start();
  drawTime.start();
  
int counter = 0;
  for(int i=0;i<drawmax;i++) 
   {

    QElapsedTimer waitTime;
    waitTime.restart();

    //while(waitTime.elapsed() < 15)

    if(i == 0)
    {
        //loopTime->start();
        qDebug() << "Starting loopTime took: " << loopTime.elapsed();
    }

    if(loopTime.elapsed() > 17)
    {

        qDebug() << "Loop time took too long";


    }
    counter = counter % 8;
    //time debugging
    qDebug() << "One loop took: " << loopTime.elapsed() << " milliseconds";
    loopTime.restart();

	c= QString("%1").arg(counter);
	listener->port->write(qPrintable(c),1);
    qDebug() << "Current c: " << c;
	qDebug() << "counter: " << counter;
	qDebug() << "\n";
    qDebug() << "Read from " << *m_serial_device << listener->port->readAll();

    //TIME HOW LONG IT TAKES TO DRAW TO THE SCREEN
    drawTime.restart();
    painter.drawPixmap(0,0, m_pixmap[counter]);
    qDebug() << "One draw took: " << drawTime.elapsed();
	   
    //SWAP THE BUFFER
    QElapsedTimer swapTime;
    swapTime.restart();
    swapBuffers();
    qDebug() << "swap time took: " << swapTime.elapsed();
     
    //FLUSH THE BUFFER
    QElapsedTimer flushTime;

    flushTime.restart();
    glFlush();
    qDebug() << "flush time took: " << flushTime.elapsed();
	counter++;
      
    }
  close();
}  
void Widget::resizeGL(int width, int height)
{
  glViewport(0, 0, width, height);
  aspect = (qreal)width / (qreal)(height ? height : 1);
  perspectiveProjection();
  textx=(width/2)-50;
  texty=(width/2)+50;
}

void Widget::perspectiveProjection()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
#ifdef QT_OPENGL_ES
  glFrustumf(-aspect, +aspect, -1.0, +1.0, 4.0, 15.0);
#else
  glFrustum(-aspect, +aspect, -1.0, +1.0, 4.0, 15.0);
#endif
  glMatrixMode(GL_MODELVIEW);
  glOrtho(0, width(), height(), 0, -1, 1);
}

void Widget::startShow()
{
  m_startTime = QDateTime::currentDateTime();
  QTimer::singleShot(0, this, SLOT(update())); 
  //  m_timer->start();
  if(m_first_run)
    {
      qDebug() << "First Run";
      m_first_run=false;
    }
}


