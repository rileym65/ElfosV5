#include "header.h"

byte extendFile(word fildes) {
  dword au;
  dword next;
  au = seekEnd(fildes);
  next = allocAU();
  if (cpu.df != 0) {
    return 0;
    }
  writeLump(au, next);
  ram[fildes+6] = 0; ram[fildes+7] = 0;
  ram[fildes+8] &= 0x7b;
  ram[fildes+8] |= 16;
  cpu.df = 0;
  return 0;
  }

