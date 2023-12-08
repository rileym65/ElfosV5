#include "header.h"

// type1 SSSS SSSS OOOO OOOO
// type2 SSSS SSSS  SSSS SSSS  SSSS SSSS  SOOO OOOO
// type2         S  SSSS SSSS  SSSS SSSS  SSSS SSSS

dword lmptosecofs(dword lump, word* offset) {
  word lat;
  if (fstype == 1) {
    lat = (lump >> 8) + 17;
    *offset = (lump & 0xff) << 1;
    return lat;
    }
  else {
    lat = (lump >> 7) + 17;
    *offset = (lump & 0x7f) << 2;
    return lat;
    }
  }

