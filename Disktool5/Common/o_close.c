#include "header.h"

void o_close(word fildes) {
  if ((ram[fildes+8] & 8) == 0) {
    cpu.df = 1;
    cpu.d = 0x10;
    return;
    }
  checkWrite(fildes);
  ram[fildes+8] &= 0xf7;
  if ((ram[fildes+8] & 0x10) == 0) {
    cpu.df = 0;
    return;
    }
  loadDirEnt(fildes, 0);
  ram[cpu.r[0x0a]+4] = ram[fildes+6];
  ram[cpu.r[0x0a]+5] = ram[fildes+7];
  ram[cpu.r[0x0a]+6] |= 0x10;
  setDateTime(cpu.r[0x0a]);
  saveSysSec();
  }

