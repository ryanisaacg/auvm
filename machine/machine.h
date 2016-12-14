#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define RAM_SIZE 1048576
#define REGISTER 0
#define REGISTER_VALUE 1
#define POINTER 2
#define CONSTANT 3

typedef unsigned char ubyte;
typedef char byte;
typedef int32_t number;

FILE *disk;
ubyte *ram, *disk_buffer;
size_t disk_size;
number registers[8];
