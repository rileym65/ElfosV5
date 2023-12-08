#include "header.h"

void o_delete(char* path) {
  word addr;
  dword au;
  while (*path == ' ') path++;
  addr = findDirent(path);
  if (cpu.df != 0) {
    cpu.d = 0x0c;
    return;
    }
  if (ram[addr+6] & 1) {
    cpu.d = 0x13;
    cpu.df = 1;
    return;
    }
  au = (ram[addr] << 24) | (ram[addr+1] << 16) |
       (ram[addr+2] << 8) | ram[addr+3];
  ram[addr] = 0;
  ram[addr+1] = 0;
  ram[addr+2] = 0;
  ram[addr+3] = 0;
  saveSysSec();
  delChain(au);
  }

