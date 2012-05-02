#ifndef WIDGET_H
#define WIDGET_H

#include <QGLWidget>
#include <QDateTime>
#include <QPointer>

class Widget : public QGLWidget
{
    Q_OBJECT

public:
    explicit Widget(int delay_before, int delay_after, int num_sets, int num_per_set, QWidget *parent = 0);
    ~Widget();

    enum State {Stopped = 0, Drawing, EndOfSet, WaitingAfter};

protected:
    void paintEvent(QPaintEvent *);

private:
    QTimer * m_timer;
    QDateTime m_startTime, m_endTime;
    QList<QPixmap> m_pixmap;

    unsigned int m_current;
    State m_state;

    unsigned int m_delay_before;
    unsigned int m_delay_after;
    unsigned int m_num_sets;
    unsigned int m_num_per_set;

    unsigned char m_port_value;
    bool m_port_ready;

private slots:
    void startShow();
};

#endif // WIDGET_H
