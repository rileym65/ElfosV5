#include "header.h"

void o_chdir(char* path) {
  int  i;
  word addr;
  if (*path == 0) {
    strcpy(path, cwd);
    cpu.df = 0;
    return;
    }
  if (*path == '/' && *(path+1) == 0) {
    for (i=0; i<6; i++)
      ram[CWD_LUMP+i] = ram[MD_LUMP+i];
      cpu.df = 0;
      strcpy(cwd,"/");
      return;
    }
  addr = findDirent(path);
  if (cpu.df != 0) return;
  if ((ram[addr+6] & 1) != 1) {
    cpu.d = 0x08;
    cpu.df = 1;
    return;
    }
  for (i=0; i<6; i++) {
    ram[CWD_LUMP+i] = ram[addr+i];
    }
  if (*path == '/') {
    strcpy(cwd, path);
    }
  else {
    if (cwd[strlen(cwd)-1] != '/') strcat(cwd,"/");
    strcat(cwd, path);
    }
  cpu.df = 0;
  }


