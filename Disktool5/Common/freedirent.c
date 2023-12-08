#include "header.h"

/* ***************************************************** */
/* ***** Assumes DTA has first sector of directory ***** */
/* ***************************************************** */
word freeDirent(dword au) {
  int i;
  word pos;
  dword lump;
  dword sec;
  dword newau;
  byte  flag;
  pos = DTA;
  flag = 0xff;
  sec = au << 3;
  readSysSec(sec);
  while (flag) {
    lump = (ram[pos] << 24) | (ram[pos+1] << 16) |
           (ram[pos+2] << 8) | ram[pos+3];
    if (lump == 0) {
      cpu.df = 0;
      return pos;
      }
    pos += 32;
    if (pos == 0x300) {
      pos = DTA;
      sec++;
      if ((sec & 7) != 0) {
        readSysSec(sec);
        }
      else {
        next = readLump(au);
        if (next == 0x0000fefe) flag = 0;
        else {
          au = next;
          next = readLump(au);
          sec = au << 3;
          readSysSec(sec);
          }
        }
      }
    }
  newau = allocAU();
  if (cpu.df != 0) return 0xffff;
  writeLump(au, newau);
  zeroLump(newau);
  sec = newau << 3;
  readSysSec(sec);
  cpu.df = 0;
  return DTA;
  }

