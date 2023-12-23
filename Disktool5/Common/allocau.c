#include "header.h"

word allocAU() {
  dword sector;
  dword pos;
  int   checkfull;
  sector = 17;
  checkfull = 0;
  while (1) {
    readSysSec(sector);
    pos = 0x00;
    while (pos < 0x200) {
      if (fstype == 1) {
        if (ram[DTA+pos] != 0xff && ram[DTA+pos+1] != 0xff) checkfull = -1;
        if (ram[DTA+pos] == 0x00 && ram[DTA+pos+1] == 0x00) {
          ram[DTA+pos] = 0xfe;
          ram[DTA+pos+1] = 0xfe;
          saveSysSec();
          cpu.df = 0;
          return (pos / 2) + (256 * (sector - 17));
          }
        if (checkfull != 0 && ram[DTA+pos] == 0xff && ram[DTA+pos+1] == 0xff) {
          cpu.df = 1;
          cpu.d = 0xf;
          return 0;
          }
        pos += 2; 
        } 
      else {
        if (ram[DTA+pos] != 0xff && ram[DTA+pos+1] != 0xff &&
            ram[DTA+pos+2] != 0xff && ram[DTA+pos+3] != 0xff) {
          checkfull = -1;
          }
        if (ram[DTA+pos] == 0x00 && ram[DTA+pos+1] == 0x00 &&
            ram[DTA+pos+2] == 0x00 && ram[DTA+pos+3] == 0x00) {
          ram[DTA+pos] = 0x00;
          ram[DTA+pos+1] = 0x00;
          ram[DTA+pos+2] = 0xfe;
          ram[DTA+pos+3] = 0xfe;
          saveSysSec();
          cpu.df = 0;
          return (pos / 4) + (128 * (sector - 17));
          }
        if (checkfull != 0 && ram[DTA+pos] == 0xff && ram[DTA+pos+1] == 0xff &&
            ram[DTA+pos+2] == 0xff && ram[DTA+pos+3] == 0xff) {
          cpu.df = 1;
          cpu.d = 0xf;
          return 0;
          }
        pos += 4; 
        }
      }
    sector++; 
    } 
  } 

