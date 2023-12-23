#include <time.h>
#include <sys/time.h>
#include "header.h"

void setDateTime(word dirent) {
  word d;
  word t;
  struct tm dt;
  time_t tim;
  tim = time(NULL);
  localtime_r(&tim, &dt);
  ram[K_MONTH] = dt.tm_mon + 1;
  ram[K_DAY] = dt.tm_mday;
  ram[K_YEAR] = dt.tm_year + 1900 - 1972;
  ram[K_HOUR] = dt.tm_hour;
  ram[K_MINUTE] = dt.tm_min;
  ram[K_SECOND] = dt.tm_sec;
  d= (ram[K_MONTH] & 0x0f) << 5;
  d|= (ram[K_DAY] & 0x1f);
  d|= (ram[K_YEAR] & 0x7f) << 9;
  ram[dirent+7] = (d>> 8) & 0xff;
  ram[dirent+8] = d& 0xff;
  t= (ram[K_HOUR] & 0x1f) << 11;
  t|= (ram[K_MINUTE] & 0x3f) << 5;
  t|= (ram[K_SECOND] >> 1);
  ram[dirent+9] = (t>> 8) & 0xff;
  ram[dirent+10] = t & 0xff;
  }

// Byte 07h   Byte 08h
// 7654 3210  7654 3210
// |______||____||____|
//   YEAR    MO    DY

// Byte 09h   Byte 0Ah
// 7654 3210  7654 3210
// |____||______||____|
//   HR    MIN    SEC/2

