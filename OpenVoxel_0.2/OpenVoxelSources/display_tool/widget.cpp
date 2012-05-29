#include "widget.h"
#include "helpers.h"
#include <QTimer>
#include <QDebug>
#include <QShortcut>
#include <QtGui>

#include <QTcpSocket>

#include "tcpclient.h" // tcp for camera

#include "qextserialport.h" // serial for arduino
#include "PortListener.h" // serial for arduino
#include "config.h"


Widget::Widget(int delay_before, int delay_after, int num_images, QString *serial_device, QString *camera_ip, QString *image_prefix, bool interlace_levels, bool numbers, QWidget *parent, QGLFormat *format)
  :QGLWidget(*format, parent)
  , m_current(0)
  , m_state(Stopped)
  , m_delay_before(delay_before)
  , m_delay_after(delay_after)
  , m_num_images(num_images)
  , m_port_value(0x0)
  , m_first_run(true)
  , m_serial_device(serial_device)
  , m_camera_ip(camera_ip)
  , m_image_prefix(image_prefix)
  , m_interlace_levels(interlace_levels)
  , m_numbers(numbers)
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
  cam_sock = new tcpClient(this);

if(interlace_levels)
    m_direction=2;  // every other frame
  else
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

      qDebug() << "Loading image:" << i << "=" << ret;
    }

  

  m_timer = new QTimer(this);
  m_timespent= new QElapsedTimer();
  m_timelastdraw= new QElapsedTimer();
  connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
  m_timer->setInterval(50); // 20 times a second we paint 3 frames 
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
  int drawmax=4;
  QString c;
  QPainter painter(this);
  int Txoff=0,Tyoff=0;
  if(m_total==0)
    {
      m_timespent->restart();
    }
  else if (m_total>59)
    {
      m_total=0;
      qDebug() << "drew 60 in " << m_timespent->elapsed() << " milliseconds";
      return;
    }
  for(int i=0;i<drawmax;i++) // every 50 ms we draw three images for 60 images per second
    {
      //wait till we're ready...
      while( (m_timespent->elapsed()) < (m_total*(1000/60)) )
	{
	  m_total=m_total; // do nothing
	}
      
      qDebug() << "START:" << QDateTime::currentDateTime().toString("hh:mm:ss.zzz");
      qDebug() << "painting level: " << m_current; 
      painter.drawPixmap(0,0, m_pixmap[m_current]);
      if(drawmax-1>i) // dont swap after the last draw just flush
	swapBuffers();
      // 	  glFlush();
      c= QString("%1").arg(m_total%((m_num_images*2)-1));
      listener->port->write(qPrintable(c),1);
      qDebug() << "Sent to " << *m_serial_device << c << " starting at level " << m_current; 
      m_current+=m_direction;
      m_total++;
      if(m_current > (m_num_images-1) )
	{
	  m_current = m_num_images-1;
	  m_direction = -m_direction;
	}
      else if(m_current < 0)
	{
	  m_current = 0;
	  m_direction = -m_direction;
	}

    }

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
  m_timer->start();
  if(m_first_run)
    {
      qDebug() << "First Run";
      m_first_run=false;
    }
}

