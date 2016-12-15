#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define RAM_SIZE 1048576

typedef unsigned char ubyte;
typedef char byte;
typedef int32_t number;

FILE *disk;
ubyte *ram, *disk_buffer, *bios;
size_t disk_size, bios_size;
number registers[8];
number register_compare;
