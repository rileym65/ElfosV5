#include "header.h"

void d_ideread(dword sector, word addr) {
  sector *= 512;
  lseek(disk, sector, SEEK_SET);
  read(disk, ram+addr, 512);
  }
