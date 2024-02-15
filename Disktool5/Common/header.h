#ifndef _CHEADER_H
#define _CHEADER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#ifdef _MSC_VER
// [RLA] Windows MSVC version ...
#include <stdint.h>
#include <io.h>
#include <fcntl.h>
typedef uint8_t byte;
#define _BYTE
typedef uint16_t word;
#define _WORD
typedef uint32_t dword;
#define _DWORD
#define close _close
#define open _open
#define read _read
#define write _write
#define lseek _lseek
#else
// [RLA] Linux verson ...
#include <unistd.h>
#define O_BINARY 0
#endif

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

#define DTA       0x100
#define DEF_LUMP  0x41f
#define DEF_EOF   0x423
#define CWD_LUMP  0x436
#define CWD_EOF   0x43a
#define CWD_FLAGS 0x43c
#define MD_LUMP   0x42f
#define MD_EOF    0x433
#define MD_FLAGS  0x435
#define K_MONTH   0x475
#define K_DAY     0x476
#define K_YEAR    0x477
#define K_HOUR    0x478
#define K_MINUTE  0x479
#define K_SECOND  0x47a

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
LINK DIRENT dirents[1024];
LINK int    numDirents;

LINK dword dir_lump;
LINK dword dir_nextLump;
LINK byte  dir_lastLump;
LINK dword dir_sector;
LINK word  dir_eof;
LINK byte  dir_snum;
LINK byte  dir_eofsec;
LINK char  cwd[128];
LINK char  def[128];
LINK dword currentSector;

extern word  allocAU();
extern void  checkWrite(word fildes);
extern word  create(dword dirAu,byte flags,char *name);
extern void  d_ideread(dword sector, word addr);
extern void  d_idewrite(dword sector, word addr);
extern void  delChain(dword au);
extern byte  extendFile(word fildes);
extern int   findDirent(char* path);
extern void  dirEmpty(dword au);
extern word  freeDirent(dword au);
extern dword lmptosec(dword lump);
extern dword lmptosecofs(dword lump, word* offset);
extern void  loadDirEnt(word fildes, byte flags);
extern void  nextSec(word fildes);
extern void  o_chdir(char* path);
extern void  o_close(word fildes);
extern void  o_delete(char* path);
extern void  o_mkdir(char* path);
extern byte  o_open(char* filename, word fildes, byte flags);
extern word  o_read(word fildes, byte* buffer, word count);
extern void  o_rename(char* srcfile, char* dstfile);
extern void  o_rmdir(char* path);
extern byte  o_write(word fildes, byte* buffer, word count);
extern dword openCD();
extern dword openMD();
extern dword readLump(dword lump);
extern void  readSysSec(dword sector);
extern void  saveSysSec();
extern word  searchDir(dword au, char* name);
extern dword seekEnd(word fildes);
extern void  setDateTime(word dirent);
extern void  setfileflags(dword fildes);
extern dword sectolmp(dword sec);
extern char* splitPath(char* path);
extern byte  validFN(char* filename);
extern void  writeLump(dword lump, dword value);
extern void  writeSysSec(dword sector);
extern void  zeroLump(dword au);

#endif

