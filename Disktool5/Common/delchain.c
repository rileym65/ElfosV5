#include "header.h"

void delChain(dword au) {
  dword next;
  while (au != 0xfefe) {
    next = readLump(au);
    writeLump(au, 0);
    au = next;
    }
  }

