#include "header.h"

void checkWrite(word fildes) {
  word dta;
  dword sector;
  if (ram[fildes+8] & 0x01) {
    dta = (ram[fildes+4] << 8) | ram[fildes+5];
    sector = (ram[fildes+15] << 24) | (ram[fildes+16] << 16) |
             (ram[fildes+17] << 8) | ram[fildes+18];
    d_idewrite(sector, dta);
    ram[fildes+8] &= 0xfe;
    }
  }

