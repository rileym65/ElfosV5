#include "header.h"

word o_read(word fildes, byte* buffer, word count) {
  int  i;
  word bytesRead;
  word size;
  word ptr;
  word dta;
  word eof;
  byte result;
  dword ofs;
  dword sec;
  byte  flags;
  flags = ram[fildes+8];
  if ((flags & 8) == 0) {
    cpu.df = 1;
    return 0x10;
    }
  bytesRead = 0;
  result = 0;
  ofs = (ram[fildes+0] << 24) | (ram[fildes+1] << 16) |
        (ram[fildes+2] << 8) | ram[fildes+3];
  eof = (ram[fildes+6] << 8) | ram[fildes+7];
  dta = (ram[fildes+4] << 8) | ram[fildes+5];
  while (count > 0) {
    ptr = ofs & 0x01ff;
    if (ram[fildes+8] & 0x80) {
      size = (eof & 0x1ff) - ptr;
      if (size == 0) count = 0;
      }
    else
      size = 512 - ptr;
    if (count < size) size = count;
    count -= size;
    bytesRead += size;
    ofs += size;
    for (i=0; i<size; i++) *buffer++ = ram[dta+ptr++];
    if ((ofs & 0x1ff) == 0) {
      ram[fildes+2] = (ofs >> 8) & 0xff;
      nextSec(fildes);
      if (cpu.df != 0) {
        count = 0;
        result = 1;
        }
      }
    }
  ram[fildes+0] = (ofs >> 24) & 0xff;
  ram[fildes+1] = (ofs >> 16) & 0xff;
  ram[fildes+2] = (ofs >> 8) & 0xff;
  ram[fildes+3] = ofs & 0xff;
  cpu.r[0xc] = bytesRead;
  cpu.df = result;
  return bytesRead;
  }

