#include "header.h"


byte o_write(word fildes, byte* buffer, word count) {
  int  i;
  word bytesWritten;
  word size;
  word ptr;
  word dta;
  word eof;
  byte result;
  dword ofs;
  dword sec;
  word oldeof;
  byte  flags;
  flags = ram[fildes+8];
  if ((flags & 8) == 0) {
    cpu.df = 1;
    cpu.d = 0x10;
    return 0x10;
    }
  if (ram[fildes+8] & 0x02) {
    cpu.df = 1;
    cpu.d = 0x01;
    return 0x01;
    }
  ram[fildes+8] |= 0x11;
  bytesWritten = 0;
  result = 0;
  ofs = (ram[fildes+0] << 24) | (ram[fildes+1] << 16) |
        (ram[fildes+2] << 8) | ram[fildes+3];
  eof = (ram[fildes+6] << 8) | ram[fildes+7];
  dta = (ram[fildes+4] << 8) | ram[fildes+5];
  while (count > 0) {
    ptr = ofs & 0x01ff;
    size = 512 - ptr;
    if (count < size) size = count;

    if (ram[fildes+8] & 0x80) {
      if (size+ptr >= (eof & 0x1ff)) {
        oldeof = eof;
        eof = (eof & 0xe00) + (ptr+size);
        if ((eof & 0xe00) != (oldeof & 0xe00)) {
          ram[fildes+8] &= 0x7f;
          if (eof > 0xfff) {
            extendFile(fildes);
            eof = (ram[fildes+6] << 8) | ram[fildes+7];
            }
          }
        ram[fildes+6] = (eof >> 8) & 0x0f;
        ram[fildes+7] = eof & 0xff;
        }
      }

    count -= size;
    bytesWritten += size;
    ofs += size;
    for (i=0; i<size; i++) ram[dta+ptr++] = *buffer++;

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
  cpu.r[0xc] = bytesWritten;
  cpu.df = result;
  return bytesWritten;
  }

