#include "header.h"

void o_rename(char* srcfile, char* dstfile) {
  int i;
  word addr;
  dword au;
  while (*srcfile == ' ') srcfile++;
  addr = findDirent(srcfile);
  if (cpu.df != 0) {
    cpu.d = 0x0c;
    return;
    }
  i = 0;
  while (*dstfile != 0) {
    ram[addr+0x0c+i] = *dstfile++;
    i++;
    }
  ram[addr+0x0c+i] = 0;
  saveSysSec();
  cpu.df = 0;
  }

