#include "header.h"

word searchDir(dword au, char* name) {
  int i;
  word pos;
  dword lump;
  dword sec;
  dword next;
  byte  flag;
  pos = DTA;
  flag = 0xff;
  sec = au <<3;
  readSysSec(sec);
  while (flag) {
    lump = (ram[pos] << 24) | (ram[pos+1] << 16) |
           (ram[pos+2] << 8) | ram[pos+3];
    if (lump != 0) {
      i = 0;
      while (i < 20 && ram[pos+0x0c+i] == name[i]) {
        if (name[i] == 0 || name[i] == 32) {
          cpu.df = 0;
          return pos;
          }
        i++;
        }
      }
    pos += 32;
    if (pos == 0x300) {
      pos = DTA;
      sec++;
      if ((sec & 7) != 0) {
        readSysSec(sec);
        }
      else {
        next = readLump(au);
        if (next == 0x0000fefe) flag = 0;
        else {
          au = next;
          next = readLump(au);
          sec = au << 3;
          readSysSec(sec);
          }
        }
      }
    }
  cpu.df = 1;
  return 0xffff;
  }

