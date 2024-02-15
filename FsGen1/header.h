#ifndef _HEADER_H
#define _HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

#ifdef _MSC_VER
// [RLA] Windows Microsoft Visual C version ...
#include <stdint.h>
#include <io.h>
typedef uint32_t dword;
typedef uint16_t word;
typedef uint8_t byte;
#define open _open
#define close _close
#define lseek _lseek
#define write _write
#else
// [RLA] And everything else is the Linux version ...
#include <unistd.h>
typedef u_int32_t dword;
typedef u_int16_t word;
typedef u_int8_t byte;
#define O_BINARY 0
#endif

#include <unistd.h>

#ifdef MAIN
#define LINK
#else
#define LINK extern
#endif

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

