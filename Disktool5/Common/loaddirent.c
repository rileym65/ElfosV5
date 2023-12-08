#include "header.h"

void loadDirEnt(word fildes, byte flags) {
  dword dirsec;
  word  dirofs;
  dirsec = (ram[fildes+9] << 24) | (ram[fildes+10] << 16) |
           (ram[fildes+11] << 8) | ram[fildes+12];
  dirofs = ((ram[fildes+13] << 8) | ram[fildes+14]) + 0x100;
  readSysSec(dirsec);
  cpu.r[0x0a] = dirofs;
  if (flags) {
    cpu.r[0x08] = (dirsec >> 24) & 0xff;
    cpu.r[0x08] |= (dirsec >> 16) & 0xff;
    cpu.r[0x07] = (dirsec >> 8) & 0xff;
    cpu.r[0x07] |= dirsec & 0xff;
    }
  }

