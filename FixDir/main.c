#define MAIN

#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>

void d_ideread(dword sector, word addr) {
  sector *= 512;
  lseek(disk, sector, SEEK_SET);
  read(disk, ram+addr, 512);
  }

void readSysSec(dword sector) {
  d_ideread(sector, DTA);
  }

void d_idewrite(dword sector, word addr) {
  sector *= 512;
  lseek(disk, sector, SEEK_SET);
  write(disk, ram+addr, 512);
  }

dword lmptosec(dword lump) {
  return lump << 3;
  }

dword lmptosecofs(dword lump, word* offset) {
  word lat;
  if (fstype == 0) {
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

void writeSysSec(dword sector) {
  d_idewrite(sector, DTA);
  }

dword readLump(dword lump) {
  word lat;
  word offset;
  lat = lmptosecofs(lump, &offset);
  if (fstype == 0) {
    readSysSec(lat);
    return (ram[DTA+offset] << 8) | ram[DTA+offset+1];
    }
  else {
    readSysSec(lat);
    return (ram[DTA+offset] << 24) | (ram[DTA+offset+1] << 16) |
           (ram[DTA+offset+2] << 8) | ram[DTA+offset+3];
    }
  }

void showSector() {
  int y,x;
  printf("\n");
  for (y=0; y<32; y++) {
    printf("%04x: ",y*16+0x100);
    for (x=0; x<16; x++)
      printf("%02x ",ram[y*16+x+0x100]);
    printf("\n");
    }
  }

void processDirectory(dword lump, word eof) {
  int   i;
  dword sec;
  dword start;
  dword lmp;
  word eofsec;
  start = lump;
  word pos;
  word deof;
  next = lump;
  while (next != 0x0000fefe) {
    lump = next;
    next = readLump(lump);
    }
  sec = lump << 3;
  eofsec = (eof >> 9) & 0x7;
  sec += eofsec;
  printf("Dir lump: %08x, sector: %08x, EOF: %04x, eofsec: %04d\n",lump, sec, eof, eofsec);
  eof &= 0x1ff;
  readSysSec(sec);
  for (i=eof; i<512; i++) ram[DTA+i] = 0x00;
  d_idewrite(sec, DTA);
  sec++;
  for (i=0; i<512; i++) ram[DTA+i] = 0x00;
  while ((sec & 0x7) != 0) {
    d_idewrite(sec, DTA);
    sec++;
    }
  lump = start;
  next = lump;
  while (next != 0x0000fefe) {
    lump = next;
    next = readLump(lump);
    sec = lump << 3;
    do {
      d_ideread(sec, DTA);
      pos = 0;
      while (pos < 0x200) {
        lmp = (ram[DTA+pos+0] << 24) | (ram[DTA+pos+1] << 16) |
              (ram[DTA+pos+2] << 8) | ram[DTA+pos+3];
        if (lmp != 0 && (ram[DTA+pos+6] & 1)) {
          deof = (ram[DTA+pos+4] << 8) | ram[DTA+pos+5];
          i=12;
          while (ram[DTA+pos+i] != 0) {
            printf("%c", ram[DTA+pos+i]);
            i++;
            }
          printf(" - %04x\n",deof);
          ram[DTA+pos+4] = 0x0f;
          ram[DTA+pos+5] = 0xff;
          d_idewrite(sec, DTA);
          processDirectory(lmp, deof);
          d_ideread(sec, DTA);
          }
        pos += 32;
        }
      sec++;
      } while ((sec & 0x7) != 0);
    }
  }

void openMD() {
  int i;
  dword lump;
  word  eof;
  byte  flags;
  readSysSec(0);
  lump = (ram[DTA+0x12c+0] << 24) | (ram[DTA+0x12c+1] << 16) |
         (ram[DTA+0x12c+2] << 8) | ram[DTA+0x12c+3];
  eof = (ram[DTA+0x12c+4] << 8) | ram[DTA+0x12c+5];
  flags = ram[DTA+0x12c+6];
  ram[DTA+0x12c+4] = 0x0f;
  ram[DTA+0x12c+5] = 0xff;
  d_idewrite(0, DTA);
  printf("Master directory lump: %08x\n",lump);
  printf("Master directory EOF : %04x\n",eof);
  printf("Flags                : %02x\n",flags);
  processDirectory(lump, eof);
  }

int main(int argc, char** argv) {
  int i;
  int j;
  int result;
  int fildes;
  char   buffer[65535];

  disk = open("disk1.ide", O_RDWR);

  fildes = 0x2000;
  ram[fildes+4] = 0x30;
  ram[fildes+5] = 0x00;
  lastsec = 0xffffffff;

  ram[32767] = H_EOT;
  heap = 32767;
  strcpy(cwd,"/");

  himem = 20000;

  openMD();

  return 0;
  }

