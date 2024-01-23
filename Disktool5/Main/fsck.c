#include "header.h"

#define MAP_SIZE  4194304

byte map[MAP_SIZE];
int crosslinked;
int allocated;
int orphaned;
int unallocated;
int invchain;

void zeroDirectory(dword lump, word eof) {
  int   i;
  dword sec;
  word eofsec;
  next = lump;
  while (next != 0x0000fefe) {
    lump = next;
    next = readLump(lump);
    }
  sec = lump << 3;
  eofsec = (eof >> 9) & 0x7;
  sec += eofsec;
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
  }

void mapChain(dword lump) {
  int pos;
  int bit;
  dword next;
  next = lump;
  while (next != 0xfefe) {
    lump = next;
    printf(" %x",lump);
    allocated++;
    pos = lump / 8;
    bit = lump % 8;
    bit = 1 << bit;
    if ((map[pos] & bit) == bit) {
      crosslinked++;
      printf("Lump %08x is multi-allocated\n",lump);
      }
    map[pos] |= bit;
    next = readLump(lump);
    if (next == 0x00000000) {
      invchain++;
      next = 0xfefe;
      }
    if (fstype == 1 && next == 0x0000ffff) {
      invchain++;
      next = 0xfefe;
      }
    if (fstype == 2 && next == 0xffffffff) {
      invchain++;
      next = 0xfefe;
      }
    }
  printf("\n");
  }

void processDirectory(dword lump, word eof) {
  int   i;
  dword sec;
  dword start;
  dword lmp;
  word eofsec;
  word pos;
  word deof;
  dword nlmp;
  next = lump;
  while (next != 0x0000fefe) {
    lump = next;
    next = readLump(lump);
    sec = lump << 3;
    do {
      readSysSec(sec);
      pos = 0;
      while (pos < 0x200) {
        lmp = (ram[DTA+pos+0] << 24) | (ram[DTA+pos+1] << 16) |
              (ram[DTA+pos+2] << 8) | ram[DTA+pos+3];
        if (lmp != 0) {
          printf("Processing file: ");
          i=12;
          while (ram[DTA+pos+i] != 0) {
            printf("%c", ram[DTA+pos+i]);
            i++;
            }
          printf(":");
          nlmp = readLump(lmp);
          if (nlmp == 0) {
            printf("  File has invalid startuing lump. deleting\n");
            }
          else if (fstype == 1 && nlmp == 0xffff) {
            printf("  File has invalid startuing lump. deleting\n");
            }
          else if (fstype == 2 && nlmp == 0xffffffff) {
            printf("  File has invalid startuing lump. deleting\n");
            }
          else mapChain(lmp);
          readSysSec(sec);
          }
        if (lmp != 0 && (ram[DTA+pos+6] & 1)) {
          deof = (ram[DTA+pos+4] << 8) | ram[DTA+pos+5];
//          ram[DTA+pos+4] = 0x0f;
//          ram[DTA+pos+5] = 0xff;
//          d_idewrite(sec, DTA);
          processDirectory(lmp, deof);
          readSysSec(sec);
          }
        pos += 32;
        }
      sec++;
      } while ((sec & 0x7) != 0);
    }
  }

void check_lat() {
  int flag;
  dword sector;
  dword lump;
  dword lmp;
  int pos;
  int endcheck;
  int p;
  int bit;
  sector = 17;
  lmp = 0;
  endcheck = 0;
  flag = -1;
  while (flag) {
    readSysSec(sector);
    pos = 0;
    while (pos < 512) {
      if (lmp != 0xfefe) {
        if (fstype == 1) {
          lump = (ram[DTA+pos+0] << 8) | ram[DTA+pos+1];
          if (lump == 0xffff) lump |= 0xffff0000;
          }
        if (fstype == 2) {
          lump = (ram[DTA+pos+0] << 24) | (ram[DTA+pos+1] << 16) |
                 (ram[DTA+pos+2] << 8) | ram[DTA+pos+3];
          }
        if (endcheck && lump == 0xffffffff) flag = 0;
        if (lump != 0xffffffff) endcheck = -1;
        if (lump == 0x00000000) unallocated++;
        if (lump != 0 && lump != 0xffffffff) {
          p = lmp / 8;
          bit = lmp % 8;
          bit = 1 << bit;
          if ((map[p] & bit) == bit) {
            }
          else {
            printf("Orphaned Lump: %08x\n",lmp);
            ram[DTA+pos+0] = 0x00;
            ram[DTA+pos+1] = 0x00;
            if (fstype == 2) {
              ram[DTA+pos+2] = 0x00;
              ram[DTA+pos+3] = 0x00;
              }
            saveSysSec();
            orphaned++;
            }
          }
        }
      if (fstype == 1) pos += 2;
      if (fstype == 2) pos += 4;
      lmp++;
      }
    sector++;
    }
  }

void cmd_fsck() {
  int i;
  dword lump;
  word  eof;
  byte  flags;
  printf("Running file system check\n");
  crosslinked = 0;
  allocated = 0;
  orphaned = 0;
  unallocated = 0;
  invchain = 0;
  for (i=0; i<MAP_SIZE; i++) map[i] = 0;
  readSysSec(0);
  lump = (ram[DTA+0x12c+0] << 24) | (ram[DTA+0x12c+1] << 16) |
         (ram[DTA+0x12c+2] << 8) | ram[DTA+0x12c+3];
  eof = (ram[DTA+0x12c+4] << 8) | ram[DTA+0x12c+5];
  flags = ram[DTA+0x12c+6];
  printf("Master directory lump: %08x\n",lump);
  printf("Master directory EOF : %04x\n",eof);
  printf("Flags                : %02x\n",flags);
  if (eof != 0xfff) {
    printf("Master directory EOF not $fff, fixing\n");
    ram[DTA+0x12c+4] = 0x0f;
    ram[DTA+0x12c+5] = 0xff;
    d_idewrite(0, DTA);
    zeroDirectory(lump, eof);
    }
  mapChain(lump);
  processDirectory(lump, eof);
  check_lat();
  printf("Allocated lumps  : %d\n",allocated);
  printf("Unallocated lumps: %d\n",unallocated);
  printf("Orphaned lumps   : %d\n",orphaned);
  printf("Crosslinked      : %d\n",crosslinked);
  printf("Invalid chain    : %d\n",invchain);
  }

