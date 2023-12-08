#include "header.h"

dword openCD() {
  dword au;
  au = (ram[CWD_LUMP] << 24) | (ram[CWD_LUMP+1] << 16) | 
       (ram[CWD_LUMP+2] << 8) | ram[CWD_LUMP+3];
  return au;
  }
