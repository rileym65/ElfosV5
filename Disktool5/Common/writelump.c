#include "header.h"

void writeLump(dword lump, dword value) {
  word lat;
  word offset;
  lat = lmptosecofs(lump, &offset);
  readSysSec(lat);
  if (fstype == 1) {
    ram[DTA+offset] = (value >> 8) & 0xff;
    ram[DTA+offset+1] = value & 0xff;
    }
  else {
    ram[DTA+offset] = (value >> 24) & 0xff;
    ram[DTA+offset+1] = (value >> 16) & 0xff;
    ram[DTA+offset+2] = (value >> 8) & 0xff;
    ram[DTA+offset+3] = value & 0xff;
    }
  writeSysSec(lat);
  }

