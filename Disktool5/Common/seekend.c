#include "header.h"

dword seekEnd(word fildes) {
  dword au;
  dword next;
  dword size;
  loadDirEnt(fildes, 1);
  au = (ram[cpu.r[0x0a]+0] << 24) | (ram[cpu.r[0x0a]+1] << 16) |
       (ram[cpu.r[0x0a]+2] << 8) | ram[cpu.r[0x0a]+3];
  size = (ram[cpu.r[0x0a]+4] << 8) | ram[cpu.r[0x0a]+5];
  next = readLump(au);
  while (next != 0xfefe) {
    size += 4096;
    au = next;
    next = readLump(au);
    }
  cpu.r[0x08] = (au >> 16) & 0xffff;
  cpu.r[0x07] = au & 0xffff;
  cpu.r[0x0b] = (size >> 16) & 0xffff;
  cpu.r[0x0a] = size & 0xffff;
  return au;
  }

