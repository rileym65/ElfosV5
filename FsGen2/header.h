#ifndef _HEADER_H
#define _HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef MAIN
#define LINK
#else
#define LINK extern
#endif

typedef u_int32_t dword;
typedef u_int16_t word;
typedef u_int8_t byte;

LINK dword diskSize;
LINK dword sectors;
LINK dword aus;
LINK dword latSectors;
LINK dword latAus;
LINK dword dataSector;
LINK dword dataLAT;
LINK int   disk;
LINK byte  dta[512];

#endif

