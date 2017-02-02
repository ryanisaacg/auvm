#pragma once

/*
 * Intermediate Representation
 * Converts from the user-friendly Abstract Syntax Tree into a stream format
 * Adds the idea of structure to ASM concepts but is otherwise fairly low-level
 * Generate with ir_ functions
 * Emitted by the behavior defined in emitter.c
 */

#include <stddef.h>

#include "ast.h"

typedef enum { ASM, VAR_NEW, VAR_GET, VAR_SET, DEF_FUN, END_FUN, CALL_FUN, RETURN_FUN, MAIN, END_MAIN, IF_START, IF_BODY_START, ELSE_BODY_START, IF_END, WHILE_START, WHILE_BODY_START, WHILE_END } IrType;

typedef struct IrNode {
	IrType type;
	void **params;
	struct IrNode *next;
} IrNode;

//All ir functions must be between ir_start_file and ir_end_file
void ir_start_file();
void ir_end_file(FILE *out_stream);
//Pushes an asm command to the IR stream
//char is the modifier for the ASM data type
//null ('\0') character to terminate arguments
void ir_inline(char *contents);
void ir_new_var(char *name);
void ir_get_var(char *name, int reg);
void ir_set_var(char *name, NodeData data, NodeType type);
//Null-terminate the arguments
void ir_start_fun(char *name, char **args);
void ir_end_fun();
void ir_call_fun(char *name, NodeData *data, NodeType *type, size_t num_args);
void ir_return_fun(NodeData data, NodeType type);
void ir_start_main();
void ir_end_main();
void ir_if_start();
void ir_if_body_start();
void ir_else_body_start();
void ir_if_end();
void ir_while_start();
void ir_while_body_start();
void ir_while_end();
