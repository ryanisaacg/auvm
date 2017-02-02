#include "ir.h"

#include "emitter.h"
#include <stdlib.h>

#define MOVE_NODE current->next = malloc(sizeof(IrNode)); current = current->next;
#define PARAMS(n) malloc(sizeof(void*) * (n))

IrNode *root, *current;

void ir_start_file() {
	root = malloc(sizeof(IrNode));
	current = root;
}

void ir_end_file(FILE *out_stream) {
	ir_emit(root, out_stream);
}

void ir_inline(char *contents) {
	current->type = ASM;
	current->params = PARAMS(1);
	current->params[0] = contents;
	MOVE_NODE;
}

void ir_new_var(char *name) {
	current->type = VAR_NEW;
	current->params = PARAMS(1);
	current->params[0] = name;
	MOVE_NODE;
}

void ir_get_var(char *name, int reg) {
	current->type = VAR_NEW;
	current->params = PARAMS(1);
	current->params[0] = malloc(sizeof(int));
	int *box = current->params[0];
	*box = reg;
	MOVE_NODE;
}

void ir_set_var(char *name, NodeData data, NodeType type) {
	current->type = VAR_SET;
	current->params = PARAMS(3);
	current->params[0] = name;
	NodeData *d = current->params[1] = malloc(sizeof(NodeData));
	*d = data;
	NodeType *t = current->params[2] = malloc(sizeof(NodeType));
	*t = type;
}

void ir_start_fun(char *name, char **args) {
	current->type = DEF_FUN;
	current->params = PARAMS(2);
	current->params[0] = name;
	current->params[1] = args;
	MOVE_NODE;
}

void ir_end_fun() {
	current->type = END_FUN;
	MOVE_NODE;
}

void ir_call_fun(char *name, NodeData *data, NodeType *type, size_t num_args) {
	current->type = CALL_FUN;
	current->params = PARAMS(4);
	current->params = (void*[]){ name, data, type, malloc(sizeof(size_t)) };
	size_t *arg = current->params[3];
	*arg = num_args;
	MOVE_NODE;
}

void ir_return_fun(NodeData data, NodeType type) {
	current->type = RETURN_FUN;
	current->params = PARAMS(2);
	NodeData *d = current->params[0] = malloc(sizeof(NodeData));
	*d = data;
	NodeType *t = current->params[1] = malloc(sizeof(NodeType));
	*t = type;
	MOVE_NODE;
}

void ir_start_main() {
	current->type = MAIN;
	MOVE_NODE;
}

void ir_end_main() {
	current->type = END_MAIN;
	MOVE_NODE;
}

void ir_if_start() {
	current->type = IF_START;
	MOVE_NODE;
}

void ir_if_body_start() {
	current->type = IF_BODY_START;
	MOVE_NODE;
}

void ir_else_body_start() {
	current->type = ELSE_BODY_START;
	MOVE_NODE;
}

void ir_if_end() {
	current->type = IF_END;
	MOVE_NODE;
}

void ir_while_start() {
	current->type = WHILE_START;
	MOVE_NODE;
}
void ir_while_body_start() {
	current->type = WHILE_BODY_START;
	MOVE_NODE;
}

void ir_while_end() {
	current->type = WHILE_END;
	MOVE_NODE;
}
