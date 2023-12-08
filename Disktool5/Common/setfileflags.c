#include "header.h"

void setfileflags(dword fildes) {
  dword sector;
  dword next;
  dword au;
  sector = (ram[fildes+15] << 24) |
           (ram[fildes+16] << 16) |
           (ram[fildes+17] << 8) |
            ram[fildes+18];
  au = sector >> 3;
  next = readLump(au);
  if (next == 0xfefe) {
    ram[fildes+8] |= 0x04;
    if ((ram[fildes+6] & 0x0e) == (ram[fildes+2] & 0x0e)) {
      ram[fildes+8] |= 0x80;
      }
    }
  }

