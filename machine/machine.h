#pragma once

#include <stdlib.h>
#include <stdio.h>

#define RAM_SIZE 1048576

typedef unsigned char ubyte;
typedef char byte;

FILE *disk;
ubyte *ram, *disk_buffer;
size_t disk_size;
int registers[8];
