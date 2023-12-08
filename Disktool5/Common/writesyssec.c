#include "header.h"

void writeSysSec(dword sector) {
  lastsec = sector;
  d_idewrite(sector, DTA);
  }
