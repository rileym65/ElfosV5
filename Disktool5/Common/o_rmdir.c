#include "header.h"

void o_rmdir(char* path) {
  word addr;
  dword au;
  dword dirsec;
  addr = findDirent(path);
  if (cpu.df != 0) return;
  dirsec = lastsec;
  au = (ram[addr+0] << 24) | (ram[addr+1] << 16) |
       (ram[addr+2] << 8) | ram[addr+3];
  dirEmpty(au);
  if (cpu.df != 0) {
    cpu.d = 0x14;
    return;
    }
  delChain(au);
  readSysSec(dirsec);
  ram[addr+0] = 0;
  ram[addr+1] = 0;
  ram[addr+2] = 0;
  ram[addr+3] = 0;
  saveSysSec();
  cpu.d = 0;
  }

