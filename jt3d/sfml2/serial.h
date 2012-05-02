/*
 * serial.h
 * serialtool
 *
 * Created by Árpád Goretity on 18/02/2012.
 * Released into the pulic domain
 */

#ifndef __SERIAL_H__
#define __SERIAL_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

/*
 * Opens a serial port represented by the special file `device'
 * `baud' must be a baud constant from <termios.h>, like `B9600'
 * The returned file descriptor may by read(), write()'n and close()'d.
 * You may also use the byte-based serial_recv() and serial_send()
 * functions below.
 */
int serial_open(const char *device, int baud);

/*
 * Retireves a single byte from the serial port file descriptor `fd'.
 * Doesn't block if no data is available.
 */
uint8_t serial_recv(int fd);

/*
 * Writes a single character to the serial port file descriptor `fd'.
 */
void serial_send(int fd, uint8_t byte);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SERIAL_H__ */

