#include "header.h"

// type1 SSSS SSSS OOOO OOOO
// type2 SSSS SSSS  SSSS SSSS  SSSS SSSS SOOO OOOO

dword readLump(dword lump) {
  word lat;
  word offset;
  lat = lmptosecofs(lump, &offset);
  if (fstype == 1) {
    readSysSec(lat);
    lump = (ram[DTA+offset] << 8) | ram[DTA+offset+1];
    return lump;
    }
  else {
    readSysSec(lat);
    return (ram[DTA+offset] << 24) | (ram[DTA+offset+1] << 16) |
           (ram[DTA+offset+2] << 8) | ram[DTA+offset+3];
    }
  }

