#include "header.h"

void zeroLump(dword au) {
  int i;
  au = au << 3;
  for (i=0; i<512; i++) ram[DTA+i] = 0;
  for (i=0; i<8; i++) {
    writeSysSec(au);
    au++;
    }
  }

