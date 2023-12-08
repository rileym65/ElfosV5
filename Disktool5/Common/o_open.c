#include "header.h"

byte o_open(char* filename, word fildes, byte flags) {
  int i;
  int result;
  dword sec;
  dword dirsec;
  word  dirofs;
  word  dta;
  dword au;
  word next;
  word ofs;
  word addr;
  byte dflags;
  word eof;
  char dir[128];
  char *name;
  result = validFN(filename);
  if (result != 0) return result;
  addr = findDirent(filename);
  dflags = ram[addr+6];
  if (cpu.df != 0) {
    if ((flags & 1) == 0) return 0x0c;
    strcpy(dir, filename);
    name = splitPath(dir);
    if (*name == 0) {
      cpu.d = 0x11;
      cpu.df = 1;
      return 0x11;
      }
    if (cpu.df == 0) {
      au = openCD();
      }
    else {
      if (*dir == 0) au = openMD();
      else {
        addr = findDirent(dir);
        if (cpu.df != 0) return 0x0c;
        }
      au = (ram[addr+0] << 24) | (ram[addr+1] << 16) |
           (ram[addr+2] << 8) | ram[addr+3];
      }
    addr = create(au, 0, name);
    if (cpu.df != 0) return cpu.d;
    }

  eof = (ram[addr+4] << 8) | ram[addr+5];
  dirsec = lastsec;
  dirofs = addr - DTA;
  au = (ram[addr+0] << 24) | (ram[addr+1] << 16) |
       (ram[addr+2] << 8) | ram[addr+3];
  sec = au << 3;

  if (((dirent[6] & 4) != 0) && ((flags & 16) == 0)) return 0x01;
  if (((dirent[6] & 1) != 0) && ((flags & 32) == 0)) return 0x12;
  ram[fildes+0] = 0;       // offset
  ram[fildes+1] = 0;
  ram[fildes+2] = 0;
  ram[fildes+3] = 0;
  ram[fildes+6] = (eof >> 8) & 0xff;           // EOF
  ram[fildes+7] = eof & 0xff;
  ram[fildes+8] = (flags & 16) ? 10 : 8;
  ram[fildes+9] = (dirsec >> 24) & 0xff;       // dir sector
  ram[fildes+10] = (dirsec >> 16) & 0xff;
  ram[fildes+11] = (dirsec >> 8) & 0xff;
  ram[fildes+12] = dirsec & 0xff;
  ram[fildes+13] = (dirofs >> 8) & 0xff;       // dir offset
  ram[fildes+14] = dirofs & 0xff;
  sec = au << 3;
  ram[fildes+15] = (sec >> 24) & 0xff;       // current sector
  ram[fildes+16] = (sec >> 16) & 0xff;
  ram[fildes+17] = (sec >> 8) & 0xff;
  ram[fildes+18] = sec & 0xff;

  if (flags & 2) {
    next = readLump(au);
    if (next != 0xfefe) delChain(next);
    writeLump(au, 0xfefe);
    ram[fildes+6] = 0;
    ram[fildes+7] = 0;
    }

  if (flags & 4)
    seekEnd(fildes);
  else {
    dta = (ram[fildes+4] << 8) | ram[fildes+5];
    d_ideread(sec, dta);
    au = readLump(au);
    if (au == 0xfefe) {
      ram[fildes+8] |= 4;
      if ((ram[fildes+2] & 0x0e) == (ram[fildes+6] & 0x0e))
        ram[fildes+8] |= 0x80;
      }
    }

  if (flags & 8) {
    sec = (ram[fildes+15] << 9) | (ram[fildes+16] << 10) |
         (ram[fildes+11] << 8) | ram[fildes+12];
    d_ideread(sec, DTA);
    ofs = (ram[fildes+13] << 8) | ram[fildes+14];
    ram[DTA+ofs+6] |= 2;
    d_idewrite(sec, DTA);
    }
  return 0;
  }

