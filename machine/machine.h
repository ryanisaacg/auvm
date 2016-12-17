#pragma once

#include "instructions.h"
#include <stdlib.h>
#include <stdio.h>

#define RAM_SIZE (1024 * 1024 * 10)
#define STACK_START 0
#define STACK_SIZE (1024 * 1024)
#define BIOS_START (1024 * 1024)

FILE *disk;
ubyte *ram, *disk_buffer, *bios;
size_t disk_size, bios_size;
number registers[8];
number register_compare;
