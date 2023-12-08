#include "header.h"

dword openMD() {
  dword au;
  au = (ram[MD_LUMP] << 24) | (ram[MD_LUMP+1] << 16) | 
       (ram[MD_LUMP+2] << 8) | ram[MD_LUMP+3];
  return au;
  }

