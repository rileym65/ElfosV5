#include "header.h"

int findDirent(char* path) {
  word addr;
  int  pos;
  char name[32];
  dword au;
  if (*path == '/') {
    au = openMD();
    path++;
    addr = MD_LUMP;
    }
  else {
    au = openCD();
    addr = CWD_LUMP;
    }
  while (*path != 0) {
    pos = 0;
    while (*path != 0 && *path != '/') name[pos++] = *path++;
    name[pos] = 0;
    addr = searchDir(au, name);
    if (cpu.df != 0) return addr;
    if (*path == '/') {
      if (!(ram[addr+6] & 1)) {
        printf("Not a directory\n");
        cpu.df = 1;
        return 0xffff;
        }
      path++;
      if (*path != 0) {
        au = (ram[addr] << 24) | (ram[addr+1] << 16) |
             (ram[addr+2] << 8) | ram[addr+3];
        }
      }
    }
  return addr;
  }
