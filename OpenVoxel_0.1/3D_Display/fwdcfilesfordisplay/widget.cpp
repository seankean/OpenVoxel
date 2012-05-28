#include "widget.h"

#include "helpers.h"

#include <QTimer>
#include <QDebug>
#include <QShortcut>

#include <fcntl.h>   /* File control definitions */
#include <sys/io.h>   /* IO definitions */

#include "config.h"


Widget::Widget(int delay_before, int delay_after, int num_sets, int num_per_set, QWidget *parent) :
  QGLWidget(QGLFormat(QGL::DoubleBuffer), parent)
  , m_current(0)
  , m_state(Stopped)
  , m_delay_before(delay_before)
  , m_delay_after(delay_after)
  , m_num_sets(num_sets)
  , m_num_per_set(num_per_set)
  , m_port_value(0x0)
  , m_port_ready(false)
{
  // Escape shortcut
  QShortcut *sh = new QShortcut(QKeySequence(trUtf8("Esc")), this);
  connect(sh, SIGNAL(activated()), this, SLOT(close()));

  // Images
  for(unsigned int i=0; i<m_num_sets*m_num_per_set; i++)
    {
      QPixmap p;
      qDebug() << (QString("image%1.bmp").arg(i, 5, 'f', 0, '0')) ;
      bool ret = p.load(QString("image%1.bmp").arg(i, 5, 'f', 0, '0'));
      if(ret)
	m_pixmap << p;
      else
	m_pixmap << QPixmap();  // empty in case of missing images
      
      qDebug() << "Loading image:" << i << "=" << ret;
    }

  m_timer = new QTimer(this);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));

  // JT: Set the delay in milliseconds before drawing the next frame.
  float frames_per_second = 60.0f;
  m_timer->setInterval(1000.0f/frames_per_second);
  // JT ----

#ifdef USE_PARALLEL_PORT
  qDebug() << "Using parallel port";

  // Open paralel port
  if(ioperm(BASEPORT, 5, 1))
    {
      qDebug() << "PORT WRITER: Unable to acquire port access";
    }
  else
    {
      // Switch to non-blocking
      int temp = fcntl(0, F_GETFL, 0);
      fcntl(0, F_SETFL, (temp | O_NDELAY));
      m_port_ready = true;
    }
#else
  qDebug() << "NOT Using parallel port";
#endif

  // Prerender images
  QTimer::singleShot(0, this, SLOT(startShow()));
}

Widget::~Widget()
{
}

void Widget::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.fillRect(rect(), Qt::black);

  qDebug() << "State=" << m_state << "Current=" << m_current;

  switch(m_state)
    {
    case EndOfSet:
      m_timer->stop();
      m_endTime = QDateTime::currentDateTime();
      qDebug() << "START:" << m_startTime.toString("hh:mm:ss.zzz");
      qDebug() << "STOP :" << m_endTime.toString("hh:mm:ss.zzz");
      m_state = WaitingAfter;
      QTimer::singleShot(0, this, SLOT(update()));
      break;

    case WaitingAfter:
      if(m_port_value == 0x0)
        {
	  m_port_value = 0xFF;
        }
      else
        {
	  m_port_value = 0x0;
        }

      FITSleep::msleep(m_delay_after);
      startShow();
      break;

    case Drawing:
      painter.drawPixmap(0, 0, m_pixmap[m_current]);
      m_current++;
      if(m_current >= (m_num_sets * m_num_per_set))
        {
	  m_current = 0;
	  m_state = EndOfSet;
        }
      else if((m_current%m_num_per_set) == 0)
        {
	  m_state = EndOfSet;
        }

    default:;
    }
}

void Widget::startShow()
{
#ifdef USE_PARALLEL_PORT
  if(m_port_ready)
    outb(m_port_value, BASEPORT);
#endif

  FITSleep::msleep(m_delay_before);

  m_state = Drawing;
  m_startTime = QDateTime::currentDateTime();
  QTimer::singleShot(0, this, SLOT(update())); // Run imediatelly
  m_timer->start();
}


