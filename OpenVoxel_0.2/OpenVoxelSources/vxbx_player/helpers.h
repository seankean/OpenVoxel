#ifndef HELPERS_H
#define HELPERS_H

#include <QThread>


#define BASEPORT 0x378

class FITSleep : public QThread
{
public:
        static void sleep(unsigned long secs) {
                QThread::sleep(secs);
        }
        static void msleep(unsigned long msecs) {
                QThread::msleep(msecs);
        }
        static void usleep(unsigned long usecs) {
                QThread::usleep(usecs);
        }
};

#endif // HELPERS_H
