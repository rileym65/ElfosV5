#include "header.h"

void d_idewrite(dword sector, word addr) {
  sector *= 512;
  lseek(disk, sector, SEEK_SET);
  write(disk, ram+addr, 512);
  }
