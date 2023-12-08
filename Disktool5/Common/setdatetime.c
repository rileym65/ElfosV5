#include "header.h"

void setDateTime(word dirent) {
  word date;
  word time;
  date = (ram[K_MONTH] & 0x0f) << 5;
  date |= (ram[K_DAY] & 0x1f);
  date |= (ram[K_YEAR] & 0x7f) << 9;
  ram[dirent+7] = (date >> 8) & 0xff;
  ram[dirent+8] = date & 0xff;
  time = (ram[K_HOUR] & 0x1f) << 11;
  time |= (ram[K_MINUTE] & 0x3f) << 5;
  time |= (ram[K_SECOND] >> 1);
  ram[dirent+9] = (date >> 8) & 0xff;
  ram[dirent+10] = date & 0xff;
  }

// Byte 07h   Byte 08h
// 7654 3210  7654 3210
// |______||____||____|
//   YEAR    MO    DY

// Byte 09h   Byte 0Ah
// 7654 3210  7654 3210
// |____||______||____|
//   HR    MIN    SEC/2

