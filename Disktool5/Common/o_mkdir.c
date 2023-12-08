#include "header.h"

void o_mkdir(char* path) {
  word addr;
  dword au;
  dword dirsec;
  char  dir[128];
  char *name;
  addr = findDirent(path);
  if (cpu.df == 0) {
    cpu.d = 0x0e;
    cpu.df = 1;
    return;
    }
  strcpy(dir, path);
  name = splitPath(dir);
  if (*name == 0) {
    cpu.d = 0x11;
    cpu.df = 1;
    return;
    }
  printf("DF: %d   Dir=%s,   Name=%s\n",cpu.df, dir, name);
  if (cpu.df == 0) {
    au = openCD();
    }
  else {
    if (*dir == 0) au = openMD();
    else {
      addr = findDirent(dir);
      if (cpu.df != 0) return;
      }
    au = (ram[addr+0] << 24) | (ram[addr+1] << 16) |
         (ram[addr+2] << 8) | ram[addr+3];
    }
printf("Dir AU: %08x\n",au);
  create(au, 1, name);
  }

