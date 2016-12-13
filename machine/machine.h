#pragma once

#include <stdlib.h>
#include <stdio.h>

typedef unsigned char ubyte;
typedef char byte;

FILE *disk;
ubyte *ram, *disk_buffer;
size_t ram_size, disk_size;
int registers[8];
