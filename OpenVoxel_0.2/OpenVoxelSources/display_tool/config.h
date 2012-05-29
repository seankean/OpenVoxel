#ifndef CONFIG_H
#define CONFIG_H
#ifdef USE_PARALLEL_PORT
#undef USE_PARALLEL_PORT
#endif
#endif // CONFIG_H

#ifdef Q_WS_X11
//("Linux");
#include <fcntl.h>   /* File control definitions */
#include <sys/io.h>   /* IO definitions */
#endif

#ifdef Q_WS_MAC
//("Mac");
#endif

#ifdef Q_WS_QWS
//("Embedded Linux");
#endif

#ifdef Q_WS_WIN
//("Windows");
#include "windows.h"
#endif

