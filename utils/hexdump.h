#ifndef HEXDUMP_H
#define HEXDUMP_H

#include <stdio.h>
#include <ctype.h>

#define HEXDUMP_COLS 16

void hexdump(void *mem, unsigned int len);

#endif // HEXDUMP_H
