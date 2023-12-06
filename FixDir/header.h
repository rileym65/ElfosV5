#ifndef _HEADER_H
#define _HEADER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#ifdef MAIN
#define LINK
#else
#define LINK extern
#endif

#ifndef _BYTE
typedef u_int8_t byte;
#define _BYTE
#endif

#ifndef _WORD
typedef u_int16_t word;
#define _WORD
#endif

#ifndef _DWORD
typedef u_int32_t dword;
#define _DWORD
#endif

/*
error codes
01 - Attempt to write read-only file
02 - Invalid FILDES
03 - File is not executable
04 - Could not open file
05 - Memory low
06 - Attempt to seek negative position
07 - Invalid seek mode
08 - Invalid directory
09 - Invalid option
0A - Missing argument
0B - Argument format error
0C - File not found
0D - No data
0E - Could not create file/directory
0F - Disk full
10 - File not open
11 - Invalid filename
12 - Attempt to open directory invalid
13 - File is a directory
FF - Unclassified error
*/

#define DTA       0x100
#define CWD_LUMP  0x400
#define CWD_EOF   0x404
#define MD_LUMP   0x406
#define MD_EOF    0x40a

#define H_EOT     0
#define H_FREE    1
#define H_USED    2
#define H_PERM    3

#define ERR_RDONLY       0x01
#define ERR_INVFILDES    0x02
#define ERR_NOTEXEC      0x03
#define ERR_NOOPEN       0x04
#define ERR_MEMLOW       0x05
#define ERR_SEEKNEG      0x06
#define ERR_INVSEEK      0x07
#define ERR_INVDIR       0x08
#define ERR_INVOPTION    0x09
#define ERR_MISSINGARG   0x0a
#define ERR_ARGFORMAT    0x0b
#define ERR_NOTFOUND     0x0c
#define ERR_NODATA       0x0d
#define ERR_CREATEFAIL   0x0e
#define ERR_DISKFULL     0x0f
#define ERR_NOTOPEN      0x10
#define ERR_INVNAME      0x11
#define ERR_OPENDIR      0x12
#define ERR_UNKNOWN      0xff


typedef struct {
  word r[16];
  byte df;
  byte d;
  } CPU;

typedef struct {
  dword   startingAU;
  word    eof;
  byte    flags;
  word    date;
  word    time;
  byte    sflags;
  char    filename[20];
  } DIRENT;

#define FL_OPEN_CREATE    1
#define FL_OPEN_TRUNCATE  2
#define FL_OPEN_APPEND    4

#define FL_LASTLUMP  1

LINK CPU  cpu;
LINK byte dirent[32];
LINK int  disk;
LINK word next;
LINK byte fstype;
LINK dword  lastsec;

LINK byte ram[65535];
LINK word heap;
LINK word himem;

LINK dword dir_lump;
LINK dword dir_nextLump;
LINK byte  dir_lastLump;
LINK dword dir_sector;
LINK word  dir_eof;
LINK byte  dir_snum;
LINK byte  dir_eofsec;
LINK char  cwd[128];

extern void checkWrite(word fildes);
extern void delChain(dword au);
extern byte extendFile(word fildes);
extern int  findDirent(char* path);
extern word freeAU();
extern dword lmptosec(dword lump);
extern dword lmptosecofs(dword lump, word* offset);
extern byte nextSec(dword *sec, word addr);
extern void nextSector(word fildes);
extern void openDir();
extern void openCD();
extern void openMD();
extern dword readLump(dword lump);
extern void d_ideread(dword sector, word addr);
extern void readSysSec(dword sector);
extern void saveSysSec();
extern dword sectolmp(dword sec);
extern void seekEnd(word fildes);
extern byte startSec(dword au, byte* dta);
extern byte validFN(char* filename);
extern void writeLump(dword lump, dword value);
extern void d_idewrite(dword sector, word addr);
extern void writeSysSec(dword sector);
extern int o_alloc(int size, byte flags, byte mask);
extern void o_chdir(char* path);
extern void o_dealloc(word address);
extern void o_delete(char* path);
extern byte o_open(char* filename, word fildes, byte flags);
extern byte o_read(word fildes, byte* buffer, word count);
extern void o_rename(char* srcfile, char* dstfile);
extern void prepDir(word address);
extern void prepDir2();
extern word searchDir(char* name);

#endif

