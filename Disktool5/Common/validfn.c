#include "header.h"

byte validFN(char* filename) { 
  byte valid; 
  char *pos; 
  byte  slash; 
  pos = filename; 
  while (*pos != 0) { 
    if (*pos == '/') filename = pos+1; 
    pos++; 
    } 
  if (*filename == 0) return 0x11; 
  while (*filename != 0) { 
    valid = 0; 
    if (*filename >= '0' && *filename <= '9') valid = 0xff; 
    if (*filename >= 'a' && *filename <= 'z') valid = 0xff; 
    if (*filename >= 'A' && *filename <= 'Z') valid = 0xff; 
    if (*filename == '.' || *filename == '_') valid = 0xff; 
    if (*filename == '-' || *filename == '$') valid = 0xff; 
    if (valid == 0) return 0x11; 
    filename++; 
    } 
  return 0; 
  } 

// $     36
// -     45
// .     46
// /     47
// 0-9   48-57
// A-Z   65-90
// _     95
// a-z   97-122
