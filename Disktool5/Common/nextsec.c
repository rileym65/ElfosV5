#include "header.h"

void nextSec(word fildes) {
  dword sec;
  dword au;
  word  dta;
  checkWrite(fildes);
  sec = (ram[fildes+15] << 24) | (ram[fildes+16] << 16) |
        (ram[fildes+17] << 8) | ram[fildes+18];
  if ((sec & 7) == 7) {
    au = sectolmp(sec);
    au = readLump(au);
    if (au == 0x0000fefe) {
      cpu.df = 1;
      return;
      }
    sec = lmptosec(au);
    }
  else {
    sec++;
    }
  ram[fildes+15] = (sec >> 24) & 0xff;
  ram[fildes+16] = (sec >> 16) & 0xff;
  ram[fildes+17] = (sec >> 8) & 0xff;
  ram[fildes+18] = sec & 0xff;
  setfileflags(fildes);
  dta = (ram[fildes+4] << 8) | ram[fildes+5];
  d_ideread(sec, dta);
  cpu.df = 0;
  }
