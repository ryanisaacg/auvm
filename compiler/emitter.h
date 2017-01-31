#pragma once

/*
 * IR Emitter
 * Takes the IR stream and outputs it to assembly
 */

#include <stdio.h>

#include "ir.h"

void ir_emit(IrNode *root, FILE *output);
