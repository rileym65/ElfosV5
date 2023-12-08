#include "header.h"

char* splitPath(char* path) {
  int i;
  char *slash;
  char *name;
  char *init;
  init = path;
  name = path;
  while (*path != 0) {
    if (*path == '/') name = path+1;
    path++;
    }
  if (init == name) cpu.df = 0;
  else {
    *(name-1) = 0;
    cpu.df = 1;
    }
  return name;
  }

