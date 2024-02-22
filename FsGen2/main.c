#define MAIN

#include "header.h"
#include "data.h"

void d_idewrite(dword sector) {
  sector *= 512;
  lseek(disk, sector, SEEK_SET);
  write(disk, dta, 512);
  }

void zero() {
  int i;
  for (i=0; i<512; i++) dta[i] = 0;
  }

void format() {
  dword i;
  printf("Preparing disk\n");
  zero();
  for (i=0; i<sectors; i++)
    d_idewrite(i);
  }

void writeBoot() {
  int i;
  byte cs;
  word md;
  printf("Installing Boot 2.1\n");
  zero();
  for (i=0; i<sector0_size; i++) dta[i] = data[i+sector0_offset];
  dta[0x100] = (sectors >> 24) & 0xff;
  dta[0x101] = (sectors >> 16) & 0xff;
  dta[0x102] = (sectors >> 8) & 0xff;
  dta[0x103] = sectors & 0xff;
  dta[0x104] = 2;
  md = dataSector;
  dta[0x105] = (md >> 8) & 0xff;
  dta[0x106] = md & 0xff;
  dta[0x107] = 2;
  dta[0x10a] = 8;
  dta[0x10b] = (aus >> 24) & 0xff;
  dta[0x10c] = (aus >> 16) & 0xff;
  dta[0x10d] = (aus >> 8) & 0xff;
  dta[0x10e] = aus & 0xff;
  dta[0x110] = (md >> 24) & 0xff;
  dta[0x111] = (md >> 16) & 0xff;
  dta[0x112] = (md >> 8) & 0xff;
  dta[0x113] = md & 0xff;
  md /= 8;
  dta[0x12c] = (md >> 24) & 0xff;
  dta[0x12d] = (md >> 16) & 0xff;
  dta[0x12e] = (md >> 8) & 0xff;
  dta[0x12f] = md & 0xff;
  dta[0x130] = 0x0f;
  dta[0x131] = 0xff;
  dta[0x132] = 05;
  dta[0x133] = 0;
  dta[0x134] = 0;
  dta[0x135] = 0;
  dta[0x136] = 0;
  dta[0x137] = 'M';
  dta[0x138] = 'D';
  dta[0x139] = 0;
  cs = 0xff;
  for (i=0; i<511; i++) {
    cs += dta[i];
    cs = (cs << 1) | ((cs >> 7) & 0x01);
    }
  dta[0x1ff] = cs;
  d_idewrite(0);
  zero();
  for (i=0; i<sector1_size; i++) dta[i] = data[sector1_offset+i];
  d_idewrite(1);
  zero();
  for (i=0; i<sector2_size; i++) dta[i] = data[sector2_offset+i];
  d_idewrite(2);
  }

void buildLAT() {
  dword au;
  dword sector;
  word  ptr;
  word  prealloc;
  printf("Building LAT\n");
  sector = 17;
  au = 0;
  prealloc = 11; 
  while (sector != dataSector) {
    ptr = 0;
    while (ptr < 512) {
      if (latAus > 0) {
        dta[ptr++] = 0xff;
        dta[ptr++] = 0xff;
        dta[ptr++] = 0xff;
        dta[ptr++] = 0xff;
        latAus--;
        aus--;
        }
      else if (prealloc > 0) {
        if (prealloc == 8) {
          dta[ptr++] = 0x00;
          dta[ptr++] = 0x00;
          dta[ptr++] = ((dataLAT+4) >> 8) & 0xff;
          dta[ptr++] = ((dataLAT+4) & 0xff);
          }
        else {
          dta[ptr++] = 0x00;
          dta[ptr++] = 0x00;
          dta[ptr++] = 0xfe;
          dta[ptr++] = 0xfe;
          }
        prealloc--;
        aus--;
        }
      else if (au == 0xfefe) {
        dta[ptr++] = 0x00;
        dta[ptr++] = 0x00;
        dta[ptr++] = 0xfe;
        dta[ptr++] = 0xfe;
        aus--;
        }
      else if (aus > 0) {
        dta[ptr++] = 0x00;
        dta[ptr++] = 0x00;
        dta[ptr++] = 0x00;
        dta[ptr++] = 0x00;
        aus--;
        }
      else {
        dta[ptr++] = 0xff;
        dta[ptr++] = 0xff;
        dta[ptr++] = 0xff;
        dta[ptr++] = 0xff;
        }
      }
    d_idewrite(sector);
    sector++;
    }
  }

void fillDir(dword sector) {
  zero();
  while ((sector & 7) != 0) {
    d_idewrite(sector);
    sector++;
    }
  }

void buildMD() {
  int i;
  word  ptr;
  dword sector;
  printf("Creating directories\n");
  sector = dataSector;
  zero();
  for (i=0; i<md_size; i++) dta[i] = data[md_offset+i];
  dta[2] = ((dataLAT+1) >> 8) & 0xff;
  dta[3] = ((dataLAT+1) & 0xff);
  dta[34] = ((dataLAT+2) >> 8) & 0xff;
  dta[35] = ((dataLAT+2) & 0xff);
  d_idewrite(sector);
  sector++;
  fillDir(sector);
  sector = dataSector+8;
  zero();
  ptr = 2;
  for (i=0; i<bin_size; i++) dta[i] = data[bin_offset+i];
  for (i=0; i<6; i++) {                          /* 1802 version */
    dta[ptr] = ((dataLAT+i+5) >> 8) & 0xff;
    dta[ptr+1] = ((dataLAT+i+5) & 0xff);
    ptr += 32;
    }
  ptr = 4;
  dta[ptr++] = ((dir_size & 0xfff) >> 8) & 0xff;
  dta[ptr++] = (dir_size & 0xfff) & 0xff;
  ptr += 30;
  dta[ptr++] = ((chdir_size & 0xfff) >> 8) & 0xff;
  dta[ptr++] = (chdir_size & 0xfff) & 0xff;
  ptr += 30;
  dta[ptr++] = ((lbr_size & 0xfff) >> 8) & 0xff;
  dta[ptr++] = (lbr_size & 0xfff) & 0xff;
  ptr += 30;
  dta[ptr++] = ((xr_size & 0xfff) >> 8) & 0xff;
  dta[ptr++] = (xr_size & 0xfff) & 0xff;
  ptr += 30;
  dta[ptr++] = ((chmod_size & 0xfff) >> 8) & 0xff;
  dta[ptr++] = (chmod_size & 0xfff) & 0xff;

  ptr += 30;
  dta[ptr++] = ((free_size & 0xfff) >> 8) & 0xff;
  dta[ptr++] = (free_size & 0xfff) & 0xff;

  d_idewrite(sector);
  sector++;
  fillDir(sector);
  sector = dataSector+16;
  zero();
  ptr = 2;
  for (i=0; i<os_size; i++) dta[i] = data[os_offset+i];
  dta[ptr] = ((dataLAT+3) >> 8) & 0xff;
  dta[ptr+1] = ((dataLAT+3) & 0xff);
  dta[ptr+2] = ((kernel_size & 0xfff) >> 8) & 0xff;
  dta[ptr+3] = (kernel_size & 0xfff) & 0xff;
  d_idewrite(sector);
  sector++;
  fillDir(sector);
  }

void writeFile(dword sector, word offset, word size) {
  int ptr;
  zero();
  ptr = 0;
  while (size > 0) {
    dta[ptr++] = data[offset++];
    size--;
    if (ptr == 512) {
      d_idewrite(sector);
      sector++;
      ptr = 0;
      }
    }
  if (ptr != 0) d_idewrite(sector);
  }

int main(int argc, char** argv) {
  printf("FsGenv5 - FsType II\n\n");

  if (argc != 2) {
    printf("Usage: fsgen1 sizeInMB\n");
    exit(1);
    }

  diskSize = atoi(argv[1]);
  sectors = (diskSize * 1000000) / 512;
  aus = sectors / 8;

  printf("Disk Size  : %dmb\n",diskSize);
  printf("Sectors    : %d\n",sectors);
  printf("AUs        : %d\n",aus);

  if (sectors > (1 << 28)) {
    printf("Error: Disk too large\n");
    exit(1);
    }

  latSectors = (aus / 128) + 1;
  dataSector = (17 + latSectors + 8) & 0xffffff8;
  dataLAT = dataSector / 8;

  latAus = dataLAT;
  latSectors = dataSector - 17;

  printf("LAT Sectors: %d\n",latSectors);
  printf("LAT AUs    : %d\n",latAus);

  disk = open("disk1.ide", O_RDWR | O_TRUNC | O_CREAT | O_BINARY, 0660);
  if (disk < 0) {
    perror("Could not create disk file");
    exit(1);
    }

  format();
  writeBoot();
  buildLAT();
  buildMD();
  printf("Installing kernel\n");
  writeFile(dataSector+24, kernel_offset, kernel_size);
  printf("Installing binaries\n");
  writeFile(dataSector+40, dir_offset, dir_size);
  writeFile(dataSector+48, chdir_offset, chdir_size);
  writeFile(dataSector+56, lbr_offset, lbr_size);
  writeFile(dataSector+64, xr_offset, xr_size);
  writeFile(dataSector+72, chmod_offset, chmod_size);

  writeFile(dataSector+80, free_offset, free_size);

  return 0;
  }

