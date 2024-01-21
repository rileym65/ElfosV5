#define MAIN

#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include <time.h>

char* getNumber(char* buffer, int* dest) {
  int ret;
  ret = 0;
  while (*buffer == ' ') buffer++;
  if (*buffer == '$') {
    buffer++;
    while ((*buffer >= '0' && *buffer <= '9') ||
           (*buffer >= 'a' && *buffer <= 'f') ||
           (*buffer >= 'A' && *buffer <= 'F')) {
      ret <<= 4;
      if (*buffer >= '0' && *buffer <= '9') ret += (*buffer - '0');
      if (*buffer >= 'a' && *buffer <= 'f') ret += (*buffer - 87);
      if (*buffer >= 'A' && *buffer <= 'F') ret += (*buffer - 55);
      buffer++;
      }
    }
  else {
    while (*buffer >= '0' && *buffer <= '9') {
      ret *= 10;
      if (*buffer >= '0' && *buffer <= '9') ret += (*buffer - '0');
      buffer++;
      }
    }
  *dest = ret;
  return buffer;
  }

void show_dir(dword au,byte flags) {
  int i;
  word pos;
  dword lump;
  dword sec;
  dword next;
  dword size;
  byte  flag;
  byte  len;
  pos = 0;
  sec = au << 3;
  next = readLump(au);
  readSysSec(sec);
  flag = 0xff;
  numDirents = 0;
  while (flag) {
    lump = (ram[DTA+pos] << 24) | (ram[DTA+pos+1] << 16) |
           (ram[DTA+pos+2] << 8) | ram[DTA+pos+3];
if (lump & 0xffff0000) {
printf("Bad entry\n");
lump = 0;
flag = 0;
}
    if (lump != 0) {
      dirents[numDirents].startingAU = lump;
      dirents[numDirents].eof = ram[DTA+pos+4]*256+ram[DTA+pos+5];
      dirents[numDirents].flags = ram[DTA+pos+6];
      dirents[numDirents].date = ram[DTA+pos+7]*256+ram[DTA+pos+8];
      dirents[numDirents].time = ram[DTA+pos+9]*256+ram[DTA+pos+10];
      i = 0x0c;
      while (ram[DTA+pos+i] != 0) {
        dirents[numDirents].filename[i-12] = ram[DTA+pos+i];
        i++;
        }
      dirents[numDirents].filename[i-12] = 0;
      numDirents++;
      }
    pos += 32;
    if (pos == 512) {
      pos = 0;
      sec++;
      if ((sec & 7) != 0) {
        
        readSysSec(sec);
        }
      else {
        if (next == 0x0000fefe) flag = 0;
        else {
          au = next;
          next = readLump(next);
          sec = au << 3;
          readSysSec(sec);
          }
        }
      }
    }
  if ((flags & 1) == 0) {
    pos = 0;
    for (i=0; i<numDirents; i++) {
      printf("%s",dirents[i].filename);
      len = strlen(dirents[i].filename);
      if (dirents[i].flags & 1) {
        printf("/");
        len++;
        }
      while ((len % 20) != 0) { printf(" "); len++; }
      pos += len;
      if (pos >= 80) { printf("\n"); pos = 0; }
      }
    if (pos != 0) printf("\n");
    }
  else {
    for (i=0; i<numDirents; i++) {
      printf("%s",dirents[i].filename);
      len = strlen(dirents[i].filename);
      if (dirents[i].flags & 1) {
        printf("/");
        len++;
        }
      while ((len % 20) != 0) { printf(" "); len++; }
      printf("  ");
      if (dirents[i].flags & 2) printf("*"); else printf(" ");
      if (dirents[i].flags & 4) printf("W"); else printf(" ");
      if (dirents[i].flags & 8) printf("H"); else printf(" ");
      if (dirents[i].flags & 16) printf("A"); else printf(" ");
      printf("  ");
      printf("%02d/",(dirents[i].date >> 5) & 0xf);
      printf("%02d/",dirents[i].date & 0x1f);
      printf("%04d ",1972 + ((dirents[i].date >> 9) & 0x7f));
      printf("%02d:",(dirents[i].time >> 11) & 0x1f);
      printf("%02d:",(dirents[i].time >>  5) & 0x3f);
      printf("%02d  ",(dirents[i].time & 0x1f) * 2);
      next = dirents[i].startingAU;
      size = dirents[i].eof;
      while (next != 0xfefe) {
        next = readLump(next);
        if (next != 0xfefe) size += 4096;
        }
      printf("%d",size);
      printf("\n");
      }
    }
  }

void cmd_dir() {
  dword au;
  au = openCD();
  show_dir(au,0);
  }

void cmd_dirs(char* buffer) {
  int i;
  word addr;
  dword au;
  byte  flags;
  flags = 0;
  while (*buffer == ' ') buffer++;
  while (*buffer == '-') {
    buffer++;
    if (*buffer == 'l') { flags |= 1; buffer++; }
    else if (*buffer == 'n') { flags |= 2; buffer++; }
    while (*buffer == ' ') buffer++;
    }
  addr = findDirent(buffer);
  if (cpu.df == 0) {
    if (ram[addr+6] & 1) {
      au = (ram[addr] << 24) | (ram[addr+1] << 16) |
           (ram[addr+2] << 8) | ram[addr+3];
 
      show_dir(au,flags);
      }
    else {
      i = 0;
      while (ram[addr+0x0c+i] != 0) { printf("%c",ram[addr+0x0c+i]); i++; }
      printf("\n");
      }
    }
  }

void cmd_copy(char* buffer) {
  int i;
  int pos;
  int  fileS;
  int  fileD;
  int  count;
  word direntS;
  word direntD;
  char src[128];
  byte tmp[256];
  pos = 0;
  while (*buffer != 0 && *buffer != ' ') src[pos++] = *buffer++;
  src[pos] = 0;
  while (*buffer == ' ') buffer++;
  printf("%s --> %s\n", src, buffer);
  direntS = 0x1000;
  ram[direntS+4] = 0x20;
  ram[direntS+5] = 0x00;
  direntD = 0x3000;
  ram[direntD+4] = 0x40;
  ram[direntD+5] = 0x00;

  if (*buffer != '[' && *src != '[') {
    }

  else if (*buffer == '[' && *src != '[') {
    strcpy((char*)tmp, buffer+1);
    strcpy(buffer, (char*)tmp);
    for (i=0; i<strlen(buffer); i++)
      if (buffer[i] == ']') buffer[i] = 0;
    fileD = open(buffer, O_CREAT | O_TRUNC | O_WRONLY, 0666);
    if (fileD < 0) {
      printf("Could not open %s\n", buffer);
      return;
      }
    o_open(src, direntS, 0);
    if (cpu.df != 0) {
      printf("D = %02x\n", cpu.d);
      printf("DF = %d\n", cpu.df);
      close(fileD);
      return;
      }
    while ((count = o_read(direntS, tmp, 256)) != 0) {
      if (cpu.df != 0) {
        printf("D = %02x\n", cpu.d);
        printf("DF = %d\n", cpu.df);
        close(fileD);
        return;
        }
      write(fileD, tmp, count);
      }
    o_close(direntS);
    close(fileD);
    }

  else if (*buffer != '[' && *src == '[') {
    strcpy((char*)tmp, src+1);
    strcpy(src, (char*)tmp);
    for (i=0; i<strlen(src); i++)
      if (src[i] == ']') src[i] = 0;
    fileS = open(src, O_RDONLY);
    if (fileS < 0) {
      printf("Could not open %s\n", src);
      return;
      }
    o_open(buffer, direntD, 3);
    if (cpu.df != 0) {
      printf("D = %02x\n", cpu.d);
      printf("DF = %d\n", cpu.df);
      close(fileS);
      return;
      }
    while ((count = read(fileS, tmp, 256)) != 0) {
      o_write(direntD, tmp, count);
      if (cpu.df != 0) {
        printf("D = %02x\n", cpu.d);
        printf("DF = %d\n", cpu.df);
        close(fileS);
        return;
        }
      }
    o_close(direntD);
    close(fileS);
    }

  else if (*buffer == '[' && *src == '[') {
    }

  }

void cmd_chdir(char* path) {
  char buffer[256];
  strcpy(buffer,path);
  o_chdir(buffer);
  printf("Path: %s\n",buffer);
  printf("DF=%d\n",cpu.df);
  }

void cmd_delete(char* buffer) {
  o_delete(buffer);
  printf("D=%02x   DF=%d\n",cpu.d,cpu.df);
  }

void cmd_rmdir(char* buffer) {
  o_rmdir(buffer);
  printf("D=%02x   DF=%d\n",cpu.d,cpu.df);
  }

void cmd_mkdir(char* buffer) {
  o_mkdir(buffer);
  printf("D=%02x   DF=%d\n",cpu.d,cpu.df);
  }

void cmd_rename(char* buffer) {
  int pos;
  char src[128];
  pos = 0;
  while (*buffer != 0 && *buffer != ' ') src[pos++] = *buffer++;
  src[pos] = 0;
  while (*buffer == ' ') buffer++;
  o_rename(src, buffer);
  printf("D=%02x   DF=%d\n",cpu.d,cpu.df);
  }

void cmd_chmod(char* buffer) {
  word addr;
  byte plusA,minusA;
  byte plusH,minusH;
  byte plusX,minusX;
  byte plusW,minusW;
  plusA = 0; minusA = 0;
  plusH = 0; minusH = 0;
  plusX = 0; minusX = 0;
  plusW = 0; minusW = 0;
  while (*buffer == ' ') buffer++;
  while (*buffer == '-' || *buffer == '+') {
    if (*buffer == '-') {
      buffer++;
      if (*buffer == 'a') minusA = 0xff;
      else if (*buffer == 'h') minusH = 0xff;
      else if (*buffer == 'x') minusX = 0xff;
      else if (*buffer == 'w') minusW = 0xff;
      else {
        printf("Invalid switch\n");
        return;
        }
      buffer++;
      }
    else {
      buffer++;
      if (*buffer == 'a') plusA = 0xff;
      else if (*buffer == 'h') plusH = 0xff;
      else if (*buffer == 'x') plusX = 0xff;
      else if (*buffer == 'w') plusW = 0xff;
      else {
        printf("Invalid switch\n");
        return;
        }
      buffer++;
      }
    while (*buffer == ' ') buffer++;
    }
  addr = findDirent(buffer);
  if (cpu.df != 0) {
    printf("Not found\n");
    return;
    }
  if (plusA) ram[addr+6] |= 16;
  if (plusH) ram[addr+6] |= 8;
  if (plusX) ram[addr+6] |= 2;
  if (plusW) ram[addr+6] |= 4;
  if (minusA) ram[addr+6] &= 0xef;
  if (minusH) ram[addr+6] &= 0xf7;
  if (minusX) ram[addr+6] &= 0xfd;
  if (minusW) ram[addr+6] &= 0xfb;
  saveSysSec();
  }

void cmd_touch(char* buffer) {
  int hour,minute,second,month,day,year;
  struct tm dt;
  time_t esec;
  word addr;
  addr = findDirent(buffer);
  if (cpu.df != 0) {
    printf("Not found\n");
    return;
    }
  ram[addr+6] |= 16;
  esec = time(NULL);
  localtime_r(&esec, &dt);
  hour = dt.tm_hour;
  minute = dt.tm_min;
  second = dt.tm_sec;
  month = dt.tm_mon + 1;
  day = dt.tm_mday;
  year = dt.tm_year + 1900 - 1972;
  ram[addr+7] = (year << 1) | ((month >> 3) & 0x1);
  ram[addr+8] = ((month & 7) << 5) | (day & 0x1f);
  ram[addr+9] = ((hour & 0x1f) << 3) | ((minute >> 3) & 0x7);
  ram[addr+10] = ((minute & 0x7) << 5) | ((second / 2) & 0x1f);
  saveSysSec();
  }

void cmd_heap() {
  word address;
  word size;
  address = heap;
  printf("Heap starts at: %04x\n",address);
  while (ram[address] != H_EOT) {
    printf("  %04x: ", address);
    if (ram[address] == H_FREE) printf("F  ");
    if (ram[address] == H_USED) printf("A  ");
    address++;
    size = ram[address++] * 256;
    size += ram[address++];
    printf("%5d   %04x\n", size,address);
    address += size;
    }
  printf("  %04x  EOT\n", address);
  }

void showDTA(word fildes) {
  word dta;
  int  x,y;
  dta = (ram[fildes+4] << 8) | ram[fildes+5];
  printf("DTA: %04x\n",dta);
  for (y=0; y<16; y++) {
    printf("%04x: ", dta+16*y);
    for (x=0; x<16; x++)
      printf("%02x ",ram[dta+y*16+x]);
    printf("\n");
    }
  }

void dumpSector(word address) {
  int x;
  int y;
  int a;
  printf("Sector: %d [$%x]\n",currentSector, currentSector);
  for (y=0; y<32; y++) {
    printf("%04x: ",y*16);
    for (x=0; x<16; x++) {
      a = y*16+x;
      if (x == 8) printf(" ");
      printf("%02x ",ram[a+address]);
      }
    printf(" ");
    for (x=0; x<16; x++) {
      a = y*16+x;
      if (x == 8) printf(" ");
      if (ram[a+address] >= ' ' && ram[a+address] < 127)
        printf("%c",ram[a+address]);
      else printf(".");
      }
    printf("\n");
    }
  }

void cmd_sector(char* buffer) {
  dword sector;
  getNumber(buffer, &sector);
  currentSector = sector;
  d_ideread(sector, 0x500);
  dumpSector(0x500);
  }

void cmd_au(char* buffer) {
  dword sector;
  getNumber(buffer, &sector);
  sector *= 8;
  currentSector = sector;
  d_ideread(sector, 0x500);
  dumpSector(0x500);
  }

void cmd_chain(char* buffer) {
  dword au;
  getNumber(buffer, &au);
  while (au != 0xfefe) {
    printf("%x ",au);
    au = readLump(au);
    if (fstype == 1 && au == 0xffff) au = 0xfefe;
    if (fstype == 2 && au == 0xffffffff) au = 0xfefe;
    if (au == 0) au = 0xfefe;
    }
  printf("\n");
  }

void cmd_showAu(char* buffer) {
  dword au;
  getNumber(buffer, &au);
  au = readLump(au);
  printf("%x ",au);
  printf("\n");
  }

void cmd_n() {
  currentSector++;
  d_ideread(currentSector, 0x500);
  dumpSector(0x500);
  }

void cmd_p() {
  if (currentSector > 0) currentSector--;
  d_ideread(currentSector, 0x500);
  dumpSector(0x500);
  }

void cmd_qm() {
  dumpSector(0x500);
  }

void cmd_free() {
  dword totalAUs;
  dword freeAUs;
  byte  checkEnd;
  dword au;
  dword sector;
  int   pos;
  int   flag;
  sector = 17;
  checkEnd = 0;
  readSysSec(0);
  if (fstype == 2) {
    totalAUs = (ram[DTA+0x10b] << 24) | (ram[DTA+0x10c] << 16) |
               (ram[DTA+0x10d] << 8) | ram[DTA+0x10e];
    }
  else {
    totalAUs = (ram[DTA+0x10b] << 8) | (ram[DTA+0x10c]);
    }
  freeAUs = 0;
  flag = -1;
  while (flag) {
    d_ideread(sector, 0x500);
    pos = 0x500;
    while (pos < 0x700) {
      if (fstype == 1) {
        au = (ram[pos] << 8) | ram[pos+1];
        if (au == 0xffff && checkEnd) flag = 0;
        if (au != 0xffff) checkEnd = -1;
        pos += 2;
        }
      else {
        au = (ram[pos] << 24) | (ram[pos+1] << 16) |
             (ram[pos+2] << 8) | ram[pos+3];
        pos += 4;
        if (au == 0xffffffff && checkEnd) flag = 0;
        if (au != 0xffffffff) checkEnd = -1;
        }
      if (au == 0) freeAUs++;
      }
    sector++;
    }
  printf("Total AUs : %d\n",totalAUs);
  printf("Free AUs  : %d\n",freeAUs);
  }

void cmd_stat() {
  dword totalAUs;
  dword freeAUs;
  dword sectors;
  byte  checkEnd;
  word count;
  word dirent;
  byte buffer[512];
  dword au;
  dword sector;
  dword mdAU;
  int   pos;
  int   flag;
  sector = 17;
  checkEnd = 0;
  readSysSec(0);
  printf("Filesystem Type : %d\n",ram[DTA+0x104]);
  sectors = (ram[DTA+0x100] << 24) | (ram[DTA+0x101] << 16) |
            (ram[DTA+0x102] << 8) | ram[DTA+0x103];
  mdAU = (ram[DTA+0x12c] << 24) | (ram[DTA+0x12d] << 16) |
         (ram[DTA+0x12e] << 8) | ram[DTA+0x12f];
  printf("Sectors         : %d\n",sectors);

  if (fstype == 2) {
    totalAUs = (ram[DTA+0x10b] << 24) | (ram[DTA+0x10c] << 16) |
               (ram[DTA+0x10d] << 8) | ram[DTA+0x10e];
    }
  else {
    totalAUs = (ram[DTA+0x10b] << 8) | (ram[DTA+0x10c]);
    }
  freeAUs = 0;
  flag = -1;
  while (flag) {
    d_ideread(sector, 0x500);
    pos = 0x500;
    while (pos < 0x700) {
      if (fstype == 1) {
        au = (ram[pos] << 8) | ram[pos+1];
        if (au == 0xffff && checkEnd) flag = 0;
        if (au != 0xffff) checkEnd = -1;
        pos += 2;
        }
      else {
        au = (ram[pos] << 24) | (ram[pos+1] << 16) |
             (ram[pos+2] << 8) | ram[pos+3];
        pos += 4;
        if (au == 0xffffffff && checkEnd) flag = 0;
        if (au != 0xffffffff) checkEnd = -1;
        }
      if (au == 0) freeAUs++;
      }
    sector++;
    }
  printf("Total AUs       : %d\n",totalAUs);
  printf("Free AUs        : %d\n",freeAUs);
  printf("MD AU           : %d [$%x]\n",mdAU,mdAU);
  printf("MD Sector       : %d [$%x]\n",mdAU*8,mdAU*8);
  dirent = 0x1000;
  ram[dirent+4] = 0x20;
  ram[dirent+5] = 0x00;
  o_open("/os/kernel", dirent, 0);
  if (cpu.df == 0) {
    count = o_read(dirent, buffer, 512);
    if (count > 0) {
      printf("Kernel          : ");
      printf("%d.%d.%d ",buffer[0x100], buffer[0x101], buffer[0x102]);
      printf("Build: %d ", buffer[0x103]*256+buffer[0x104]);
      printf("%d/%d/%d ",buffer[0x105], buffer[0x106], buffer[0x107]*256+buffer[0x108]);
      printf("\n");
      }
    }
  }

void cmd_type(char* path) {
  int  i;
  word count;
  word dirent;
  byte buffer[256];
  dirent = 0x1000;
  ram[dirent+4] = 0x20;
  ram[dirent+5] = 0x00;
  o_open(path, 0x1000, 0);
  if (cpu.df != 0) {
    printf("File not found\n");
    return;
    }
  while ((count = o_read(dirent, buffer, 32)) != 0) {
    for (i=0; i<count; i++) printf("%c",buffer[i]);
    }
  }

void cmd_hexdump(char* path) {
  int  i;
  int  c;
  word count;
  word dirent;
  word addr;
  byte buffer[256];
  dirent = 0x1000;
  ram[dirent+4] = 0x20;
  ram[dirent+5] = 0x00;
  o_open(path, 0x1000, 0);
  if (cpu.df != 0) {
    printf("File not found\n");
    return;
    }
  c = 0;
  addr = 0;
  printf("%04x: ",addr);
  while ((count = o_read(dirent, buffer, 32)) != 0) {
    for (i=0; i<count; i++) {
      printf("%02x ",buffer[i]);
      c++;
      if (c == 16) {
        printf("\n");
        c = 0;
        addr += 16;
        printf("%04x: ",addr);
        }
      }
    }
  printf("\n");
  }

void cmd_ex(char* buffer) {
  int addr;
  int v;
  buffer = getNumber(buffer, &addr);
  if (addr < 0 || addr > 511) {
    printf("Invalid address\n");
    return;
    }
  while (*buffer == ' ') buffer++;
  while (*buffer != 0) {
    buffer = getNumber(buffer, &v);
    if (addr < 512) {
      ram[0x500+addr] = v;
      }
    addr++;
    while (*buffer == ' ') buffer++;
    }
  }


void cmd_wrsec() {
  d_idewrite(currentSector, 0x500);
  }

void openDisk() {
  int i;
  disk = open(diskName, O_RDWR);
  if (disk < 0) {
    perror("Error opening disk");
    exit(1);
    }
  readSysSec(0);
  fstype = ram[DTA+0x104];
  printf("Filesystem type: %d\n", fstype);
  for (i=0; i<7; i++) {
    ram[CWD_LUMP+i] = ram[DTA+0x12c+i];
    ram[MD_LUMP+i] = ram[DTA+0x12c+i];
    }
  strcpy(cwd,"/");
  }

void cmd_mount(char* buffer) {
  close(disk);
  strcpy(diskName, buffer);
  openDisk();
  }

void cmd_help() {
  printf("!addr nn nn ... - Write bytes to disk buffer\n");
  printf("?au num         - Show value of specified AU\n");
  printf("au n            - Load AU n\n");
  printf("chain au        - Show shown starting with au\n");
  printf("chdir path      - Change working directory\n");
  printf("chmod opt file  - Change options on a file\n");
  printf("copy src dst    - Copy a file\n");
  printf("delete file     - Delete file\n");
  printf("dir             - Show current directory\n");
  printf("dir path        - Show directory of path\n");
  printf("free            - Show number of free AUs\n");
  printf("fsck            - Perform file system check\n");
  printf("hexdump file    - Show hex dump of file\n");
  printf("mkdir path      - Make directory\n");
  printf("mount diskfile  - Mount a new disk file\n");
  printf("n               - Next sector\n");
  printf("p               - Previous sector\n");
  printf("pwd             - Print working directory\n");
  printf("rename old new  - Rename file\n");
  printf("rmdir path      - Remove directory\n");
  printf("sector n        - Load sector n\n");
  printf("stat            - Show disk statistics\n");
  printf("touch file      - Update date/time on file\n");
  printf("type file       - Type contents of file\n");
  printf("wrsec           - Write sector buffer back to disk\n");
  printf("?               - Dump current sector\n");
  }

extern void doCommand(char* buffer);

void cmd_do(char* filename) {
  FILE* file;
  char  buffer[256];
  file = fopen(filename, "r");
  if (file == NULL) return;
  while (fgets(buffer, 255, file) != NULL) {
    while (buffer[0] != 0 && buffer[strlen(buffer)-1] < ' ')
      buffer[strlen(buffer)-1] = 0;
    printf("%s\n",buffer);
    doCommand(buffer);
    }
  fclose(file);
  }

void doCommand(char* buffer) {
  if (buffer[0] == '!') cmd_ex(buffer+1);
  if (strncmp(buffer,"?au ",4) == 0) cmd_showAu(buffer+4);
  if (strcmp(buffer,"quit") == 0) strcpy(buffer,"exit");
  if (strcmp(buffer,"heap") == 0) cmd_heap();
  if (strcmp(buffer,"cd") == 0) printf("%s\n", cwd);
  if (strcmp(buffer,"chdir") == 0) printf("%s\n", cwd);
  if (strncmp(buffer,"cd ", 3) == 0) cmd_chdir(buffer+3);
  if (strncmp(buffer,"chdir ", 6) == 0) cmd_chdir(buffer+6);
  if (strcmp(buffer,"ls") == 0) cmd_dir();
  if (strncmp(buffer,"ls ", 3) == 0) cmd_dirs(buffer+3);
  if (strcmp(buffer,"dir") == 0) cmd_dir();
  if (strncmp(buffer,"dir ", 4) == 0) cmd_dirs(buffer+4);
  if (strncmp(buffer,"del ", 4) == 0) cmd_delete(buffer+4);
  if (strncmp(buffer,"delete ", 7) == 0) cmd_delete(buffer+7);
  if (strncmp(buffer,"rename ", 7) == 0) cmd_rename(buffer+7);
  if (strncmp(buffer,"rm ", 3) == 0) cmd_delete(buffer+3);
  if (strncmp(buffer,"rmdir ", 6) == 0) cmd_rmdir(buffer+6);
  if (strncmp(buffer,"mkdir ", 6) == 0) cmd_mkdir(buffer+6);
  if (strncmp(buffer,"sector ", 7) == 0) cmd_sector(buffer+7);
  if (strncmp(buffer,"au ", 3) == 0) cmd_au(buffer+3);
  if (strncmp(buffer,"chain ", 6) == 0) cmd_chain(buffer+6);
  if (strcmp(buffer,"n") == 0) cmd_n();
  if (strcmp(buffer,"p") == 0) cmd_p();
  if (strcmp(buffer,"?") == 0) cmd_qm();
  if (strcmp(buffer,"pwd") == 0) printf("%s\n", cwd);
  if (strcmp(buffer,"/") == 0) strcpy(buffer,"exit");
  if (strcmp(buffer,"help") == 0) cmd_help();
  if (strcmp(buffer,"free") == 0) cmd_free();
  if (strcmp(buffer,"stat") == 0) cmd_stat();
  if (strncmp(buffer,"type ", 5) == 0) cmd_type(buffer+5);
  if (strncmp(buffer,"hexdump ", 8) == 0) cmd_hexdump(buffer+8);
  if (strncmp(buffer,"chmod ", 6) == 0) cmd_chmod(buffer+6);
  if (strncmp(buffer,"touch ", 6) == 0) cmd_touch(buffer+6);
  if (strncmp(buffer,"cp ", 3) == 0) cmd_copy(buffer+3);
  if (strncmp(buffer,"copy ", 5) == 0) cmd_copy(buffer+5);
  if (strncmp(buffer,"do ", 3) == 0) cmd_do(buffer+3);
  if (strcmp(buffer,"wrsec") == 0) cmd_wrsec(buffer+3);
  if (strcmp(buffer,"fsck") == 0) cmd_fsck();
  if (strncmp(buffer,"mount ", 6) == 0) cmd_mount(buffer+6);
  }

int main(int argc, char** argv) {
  int i;
  int j;
  int result;
  int fildes;
  char   buffer[65535];

  strcpy(diskName, "disk1.ide");

  i = 1;
  while (i<argc) {
    if (strcmp(argv[i], "-d") == 0) {
      i++;
      strcpy(diskName, argv[i]);
      }
    i++;
    }

  disk = open(diskName, O_RDWR);
  if (disk < 0) {
    perror("Error opening disk");
    exit(1);
    }

  fildes = 0x2000;
  ram[fildes+4] = 0x30;
  ram[fildes+5] = 0x00;
  lastsec = 0xffffffff;

  ram[32767] = H_EOT;
  heap = 32767;
  strcpy(cwd,"/");

  himem = 20000;
  currentSector = 0;

  openDisk();
//  readSysSec(0);
//  fstype = ram[DTA+0x104];
//  printf("Filesystem type: %d\n", fstype);
//  for (i=0; i<7; i++) {
//    ram[CWD_LUMP+i] = ram[DTA+0x12c+i];
//    ram[MD_LUMP+i] = ram[DTA+0x12c+i];
//    }
  
  strcpy(buffer, "");
  while (strcmp(buffer,"exit") != 0) {
    printf("\n>");
    fgets(buffer,128, stdin);
    while (strlen(buffer) > 0 && buffer[strlen(buffer) - 1] <= ' ')
      buffer[strlen(buffer)-1] = 0;
    doCommand(buffer);
    }
  
  return 0;
  }

