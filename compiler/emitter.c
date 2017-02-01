#include "emitter.h"
#include "table.h"

FILE *file;
Table *tbl;
FunctionTable *ftbl;
int label = 0;

//TODO: Fix the problem where nested ifs will override each others labels

static void emit_asm(char *string);
static void emit_var_new(char *varname);
static void emit_var_get(char *varname, int *reg);
static void emit_var_set(char *varname, NodeData *data, NodeType *type);
static void emit_start_fun(char *name, char **args);
static void emit_end_fun();
static void emit_call_fun(char *name, NodeData *data, NodeType *type, size_t *args);
static void emit_return_fun(NodeData *data, NodeType *type);
static void emit_start_main();
static void emit_end_main();
static void emit_if_start();
static void emit_if_body_start();
static void emit_else_body_start();
static void emit_if_end();
static void emit_while_start();
static void emit_while_body_start();
static void emit_while_end();

void ir_emit(IrNode *root, FILE *output) {
	file = output;
	tbl = table_new(NULL);
	ftbl = func_table_new();
	fputs("brn =0\n", output); //jump to the main
	while(root != NULL) {
		switch(root->type) {
		case ASM:
			emit_asm(root->params[0]);
			break;
		case VAR_NEW:
			emit_var_new(root->params[0]);
			break;
		case VAR_GET:
			emit_var_get(root->params[0], root->params[1]);
			break;
		case VAR_SET:
			emit_var_set(root->params[0], root->params[1], root->params[2]);
			break;
		case DEF_FUN:
			emit_start_fun(root->params[0], root->params[1]);
			break;
		case END_FUN:
			emit_end_fun();
			break;
		case CALL_FUN:
			emit_call_fun(root->params[0], root->params[1], root->params[2], root->params[3]);
			break;
		case RETURN_FUN:
			emit_return_fun(root->params[0], root->params[1]);
			break;
		case MAIN:
			emit_start_main();
			break;
		case END_MAIN:
			emit_start_main();
			break;
		case IF_START:
			emit_if_start();
			break;
		case IF_BODY_START:
			emit_if_body_start();
			break;
		case ELSE_BODY_START:
			emit_else_body_start();
			break;
		case IF_END:
			emit_if_end();
			break;
		case WHILE_START:
			emit_while_start();
			break;
		case WHILE_BODY_START:
			emit_while_body_start();
			break;
		case WHILE_END:
			emit_while_end();
			break;
		}
		root = root->next;
	}
}

static void emit_asm(char *string) {
	fputc('\n', file);
	fputs(string, file);
	fputc('\n', file);
}

static void emit_var_new(char *varname) {
	fputs(	"; Create a new variable\n"
			"mov %0 R0\n"
			"mul R0 =1024\n"
			"add R0 =5 R0\n"
			"add R0 R$0 R1\n"
			"mov R$0 R3\n"
			"mov R3 R$0\n"
			"; End variable creation", file);
	table_add(tbl, varname);
}

static void emit_var_get(char *varname, int *reg) {
	int offset = table_get(tbl, varname);
	int register_index = *reg;
	fputs(			"; Get the value of a variable into a register\n"
					"mov %0 R0\n"
					"mul R0 =1024\n"
					"add R0 =5 R0\n", file);
	fprintf(file, "add R0 =%d R0\n"
					"mov R$0 R%d", offset, register_index);
	fputs(			"; Stop getting the value of a variable\n", file);
}

static void emit_var_set(char *varname, NodeData *data, NodeType *type);
static void emit_start_fun(char *name, char **args);
static void emit_end_fun();
static void emit_call_fun(char *name, NodeData *data, NodeType *type, size_t *args);

static void emit_return_fun(NodeData *data, NodeType *type) {
	//Get the current amount of stacks
	//Multiply it by the stack size (1024 in decimal, 400 in hex)
	//Add 1 to it
	//Subtract one from the amount of stacks
	//Store it as the number of stacks
	//Get the byte indicated by the first value in the stack
	//Jump there
	//TODO: Actual return values
	fputs(	"mov %0 R0\n"
			"mul R0 =1024 R1\n"
			"add R1 =1 R1\n"
			"sub =1 R0 R0\n"
			"mov R0 %0\n"
			"gtb R$1", file);
}

static void emit_start_main() {
	fputs("lbl =0\n", file);
}

static void emit_end_main() {
	fputs("exit\n", file);
}

static void emit_if_start();
static void emit_if_body_start();

static void emit_else_body_start() {
	fprintf(file, "brn =%d\n", label + 1); 
	fprintf(file, "lbl =%d\n", label);
}

static void emit_if_end() {
	putc('\n', file);
	fprintf(file, "lbl =%d\n", label + 1);
	label += 2;
}

static void emit_while_start();
static void emit_while_body_start();
static void emit_while_end();