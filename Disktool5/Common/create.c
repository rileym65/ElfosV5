#include "header.h"

word create(dword dirAu,byte flags,char *name) {
  int   i;
  dword au;
  word  dirent;
  dword dirsec;
  au = allocAU();
  if (cpu.df != 0) return 0;
  dirent = freeDirent(dirAu);
  if (cpu.df != 0) {
    writeLump(au, 0x00000000);
    return 0;
    }
  ram[dirent+0] = (au >> 24) & 0xff;
  ram[dirent+1] = (au >> 16) & 0xff;
  ram[dirent+2] = (au >> 8) & 0xff;
  ram[dirent+3] = au & 0xff;
  if (flags & 1) {
    ram[dirent+4] = 0x0f;
    ram[dirent+5] = 0xff;
    }
  else {
    ram[dirent+4] = 0;
    ram[dirent+5] = 0;
    }
  ram[dirent+6] = flags;
  i = 12;
  while (*name != 0 && i<31) {
    ram[dirent+i] = *name++;
    i++;
    }
  ram[dirent+i] = 0;
  setDateTime(dirent);
  saveSysSec();
  dirsec = lastsec;
  zeroLump(au);
  readSysSec(dirsec);
  cpu.df = 0;
  return dirent;
  }

