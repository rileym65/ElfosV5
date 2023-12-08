#include "header.h"

void readSysSec(dword sector) {
  if (lastsec == sector) return;
  lastsec = sector;
  d_ideread(sector, DTA);
  }

