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


Widget::Widget(int delay_before, int delay_after, int max_time, int num_images, QString *serial_device, QString *camera_ip, QString *image_prefix, bool interlace_levels, bool numbers, QWidget *parent, QGLFormat *format, bool rotate)
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
{
  setAutoFillBackground(false);
  makeCurrent();
  
  // Escape shortcut
  QShortcut *sh = new QShortcut(QKeySequence(trUtf8("Esc")), this);
  connect(sh, SIGNAL(activated()), this, SLOT(close()));
  m_total=0;

  // Info
  QMessageBox::information(this, tr("Info"),tr("HIT 'ESC' to CLOSE"));  
 
  QString portName(*m_serial_device);           
  listener = new PortListener(portName);        // signals get hooked up internally
  //cam_sock = new tcpClient(this);

  if(interlace_levels)
    m_direction=2;  // every other frame
  else
    m_direction=1;

  // Images
//   for( int i=0; i<m_num_images; i++)
//     {
//       QPixmap p;
//       QString fname=*m_image_prefix;
//       fname+=QString("%1.bmp").arg(i, 5, 'f', 0, '0');
//       bool ret = p.load(fname);
//       if(ret)
// 	m_pixmap << p;
//       else
// 	m_pixmap << QPixmap();  // empty in case of missing images

//       qDebug() << "Loading image:" << i << "=" << ret;
//     }

  // create num_images empty images...

  for( int i=0; i<8; i++)
    m_pixmap << QPixmap();
  m_curr_angle = 0;
  m_draw_count = 0;
  //  m_timer = new QTimer(this);
  m_timespent= new QElapsedTimer();
  m_timelastdraw= new QElapsedTimer();
  //  connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
  //  m_timer->setInterval(50); // times a second we paint drawmax frames 
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
  QString fname=*m_image_prefix;
  QString tmp;
  QString c;
  QPainter painter(this);
  QThread t(this);
  int Txoff=0,Tyoff=0;
  if(m_total==0)
    {
      m_timespent->restart();
    }
  else if (m_total>59)
    {
      m_total=0;
      qDebug() << "drew 60 in " << m_timespent->elapsed() << " milliseconds";
    }
  
  for(int i=0;i<drawmax;i++) 
    {
      tmp=fname;
      tmp+=QString("%1.bmp").arg(m_current, 5, 'f', 0, '0');
      bool ret = m_pixmap[m_current%8].load(tmp);
      if(!ret)
	{
	  qDebug() << "FAILED on image:" << m_current << "=" << ret << "EXITING" ;
	  exit(1);
	}
      qDebug() << "Loading image:" << m_current << "=" << ret;

      if(m_total>0)
	{
	  //wait till we're ready...
	  
	  if (i%4==0)
	    Txoff=1;
	  else
	    Txoff=0;
	  
	  while( (m_timespent->elapsed()) < (16+Txoff) )
	    {
	      Txoff=Txoff;
	    }
	}
      qDebug() << "START:" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
      qDebug() << "painting level: " << m_current; 
	  
      c= QString("%1").arg(m_current);
      listener->port->write(qPrintable(c),1);
	  
      qDebug() << "Sent to " << *m_serial_device << c << " starting at level " << m_current; 
      qDebug() << "Read from " << *m_serial_device << listener->port->readAll();
	  
	  
      painter.drawPixmap(0,0, m_pixmap[m_current%8]);
	  
      if(drawmax-1>i) // dont swap after the last draw just flush
	swapBuffers();
      glFlush();
      m_current+=m_direction;
      m_total++;
      
      if(m_current > (m_num_images-1) )
	{
	  m_current = 0;
	  m_direction = -m_direction;
	}
      else if(m_current < 0)
	{
	  m_current = 0;
	  m_direction = -m_direction;
	}
      
      if(m_total==0)
	{
	  m_timespent->restart();
	}
      else if (m_total>59)
	{
	  m_total=0;
	  qDebug() << "drew 60 in " << m_timespent->elapsed() << " milliseconds";
	}
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


