/*
 * serial.c
 * serialtool
 *
 * Created by Árpád Goretity on 18/02/2012.
 * Released into the pulic domain
 */

#include "serial.h"

int serial_open(const char *device, int baud)
{	
    struct termios tio = {0};
    //memset(&tio, 0, sizeof(tio));
	
	/* Open serial port in mode `8N1', non-blocking */
	tio.c_cflag = CS8 | CREAD | CLOCAL;
	tio.c_cc[VMIN] = 1;
	tio.c_cc[VTIME] = 5;

        // NOTE: IF YOU USE NONBLOCKING YOU'RE GOING TO HAVE A BAD TIME
	//int fd = open(device, O_RDWR | O_NONBLOCK);
	int fd = open(device, O_RDWR);
	
	/*
	 * Note that `baud' is not an actual baud rate number;
	 * it's one of the constants (e. g., `B9600') in <termios.h>
	 */
	cfsetospeed(&tio, baud);
	cfsetispeed(&tio, baud);
	tcsetattr(fd, TCSANOW, &tio);
	
	return fd;
}

uint8_t serial_recv(int fd)
{
	uint8_t byte;
	read(fd, &byte, 1);
	return byte;
}

void serial_send(int fd, uint8_t byte)
{
	write(fd, &byte, 1);
}

