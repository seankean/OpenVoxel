#include <QtGui/QApplication>
#include "widget.h"
#include <QDebug>

#include <linux/i2c-dev.h>
#include <fcntl.h>   /* File control definitions */
#include <errno.h>
#include "config.h"
#include <sys/ioctl.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    // Parse and process arguments
    QString i2c_device = "";
    int delay_before = 0;
    int delay_after = 0;
    int num_sets = 1;
    int num_per_set = 1;

    char test[5];

    QStringList arguments = a.arguments();
    qDebug() << "ARGUMENTS: " << arguments;

    for(int i=0; i<arguments.count(); i++)
    {
        if(arguments.at(i) == "--i2c_device")
        {
           i2c_device = arguments.at(i+1);
           test[0] = arguments.at(i+2).toInt();
           test[1] = arguments.at(i+3).toInt();
           test[2] = arguments.at(i+4).toInt();
           test[3] = arguments.at(i+5).toInt();
           test[4] = arguments.at(i+6).toInt();
           i+=6;
        }
        else if(arguments.at(i) == "--delay_before")
        {
            if((i+1)<arguments.count())
            {
               delay_before = arguments.at(i+1).toInt();
               i++;
            }
        }
        else if(arguments.at(i) == "--delay_after")
        {
            if((i+1)<arguments.count())
            {
               delay_after = arguments.at(i+1).toInt();
               i++;
            }
        }
        else if(arguments.at(i) == "--num_sets")
        {
            if((i+1)<arguments.count())
            {
               num_sets = arguments.at(i+1).toInt();
               i++;
            }
        }
        else if(arguments.at(i) == "--num_per_set")
        {
            if((i+1)<arguments.count())
            {
               num_per_set = arguments.at(i+1).toInt();
               i++;
            }
        }
    }

    qDebug() << "i2c_device" << i2c_device;
    qDebug() << "delay_before" << delay_before;
    qDebug() << "delay_after" << delay_after;
    qDebug() << "num_sets" << num_sets;
    qDebug() << "num_per_set" << num_per_set;
    qDebug() << "test" << QString::number(test[0]) << QString::number(test[1]) << QString::number(test[2]) << QString::number(test[3]) << QString::number(test[4]);

    // Send i2c command
    if(!i2c_device.isEmpty())
    {
        int fd = open(i2c_device.toLatin1(), O_RDWR);
        if(fd < 0)
        {
            qDebug() << "I2C: error opening device:" << strerror(errno);
            return -1;
        }

        if(ioctl(fd, I2C_SLAVE, 0x1b) < 0)
        {
            qDebug() << "I2C: error selecting I2C_SLAVE:" << strerror(errno);
            close(fd);
            return -1;
        }

        // TESTS
        if(write(fd, test, 5) != 5)
        {
            qDebug() << "I2C: error writing data:" << strerror(errno);
            close(fd);
            return -1;
        }


        // Red LED off
        /*
        char buf[5] = {0x11, 0, 0, 0, 0};
        if(write(fd, buf, 5) != 5)
        {
            qDebug() << "I2C: error writing data 1:" << strerror(errno);
            close(fd);
            return -1;
        }

        // Green LED 45%
        char buf2[5] = {0x0F, 0x0, 0x0, 0x0, 0};
        if(write(fd, buf2, 5) != 5)
        {
            qDebug() << "I2C: error writing data 2:" << strerror(errno);
            close(fd);
            return -1;
        }

        // Mode 1440
        char buf3[5] = {0x1F, 0x0, 0x00, 0, 0x06};
        if(write(fd, buf3, 5) != 5)
        {
            qDebug() << "I2C: error writing data 3:" << strerror(errno);
            close(fd);
            return -1;
        }

        // Temp blue turn off
        char bufX[5] = {0x13, 0, 0, 0, 0};
        if(write(fd, bufX, 5) != 5)
        {
            qDebug() << "I2C: error writing data X:" << strerror(errno);
            close(fd);
            return -1;
        }
*/
        close(fd);
    }

    // Show window
    Widget w(delay_before, delay_after, num_sets, num_per_set);
    w.showFullScreen();

    return a.exec();
}
