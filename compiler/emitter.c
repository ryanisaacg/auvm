#include "emitter.h"

#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "table.h"

FILE *file;
Table *tbl;
FunctionTable *ftbl;
Stack *labels;
int label = 1;

static void emit_get_stack_top(); //puts the top of the stack into R0
static int var_offset(char *varname);
static void emit_var_abs_offset(char *varname);
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
	labels = malloc(sizeof(Stack));
	*labels = stack_new();
	emit_var_new("__return");
	fputs("brn =0\n", output); //jump to the main
	while(root != NULL) {
		switch(root->type) {
		case ASM:
			if(root->params != NULL) {
				emit_asm(root->params[0]);
			}
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
			emit_end_main();
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

#define STACK_SIZE 1024

static void emit_get_stack_top() {
	fputs("mul R2 =1024 R0\n", file);
}

static int var_offset(char *varname) {
	int stack = 0;
	int offset = table_get(tbl, varname, &stack);
	return offset * 4 + 4; //each variable takes up 4 bytes and the first 4 bytes are the jump return location
}

static void emit_var_abs_offset(char *varname) {
	emit_get_stack_top();
	int off = var_offset(varname);
	fprintf(file, "add =%d R0 R0\n", off);
}

static void emit_asm(char *string) {
	fputs(string, file);
	fputc('\n', file);
}

static void emit_var_new(char *varname) {
	table_add(tbl, varname);
}

static void emit_var_get(char *varname, int *reg) {
	int register_index = *reg;
	emit_var_abs_offset(varname);
	fprintf(file,	"; Get the value of a variable into a register\n"
					"mov R$0 R%d\n", register_index);
}

static void emit_var_set(char *varname, NodeData *data, NodeType *type) {
	fputs("; Set the value of a variable \n", file);
	emit_var_abs_offset(varname);
	switch(*type) {
	case WORD_NODE: {
		char *word = data->sval;
		//Setting a variable to a register value
		if(strlen(word) == 2 && word[0] == 'R' && word[1] >= '0' && word[1] <= '9') {
			fprintf(file, "mov R%d R$0\n", word[1] - '0');
		} else {
			fputs("mov R0 R3\n", file);
			emit_var_abs_offset(word);
			fputs("mov R$0 R$3\n", file);
		}
	}	break;
	case STRING_NODE:
		fprintf(stderr, "Setting variables to strings is not yet supported.\n");
		break;
	case NUMBER_NODE:
		fprintf(file, "mov =%d R$0\n", data->ival);
		break;
	case NIL_NODE:
		fprintf(stderr, "Cannot set a variable to NIL.\n");
		break;
	case ROOT_NODE:
		fprintf(stderr, "Cannot set a variable to ROOT\n");
		break;
	}
}

static void emit_start_fun(char *name, char **args) {
	func_table_add(ftbl, name, args, label);
	fprintf(file, "lbl =%d \n; start function %s\n", label, name);
	while(*args != NULL) {
		table_add(tbl, *args);
		args++;
	}
	label++;
	tbl = table_new(tbl);
	emit_var_new("__return");
}

static void emit_end_fun() {
	emit_get_stack_top();
	fputs("sub =1 R2 R2\n", file);
	fprintf(file, "gtb R$0\n");
	tbl = tbl->parent;
}

static void emit_call_fun(char *name, NodeData *data, NodeType *type, size_t *args) {
	//Get the current amount of stacks
	//Add 1 to it
	//Multiply it by the stack size (1024 in decimal, 400 in hex)
	//Add 1 to it
	//Get the current byte the CPU is looking at
	//Add 36 to the value (24 in hex) to move past the instruction when returning
	//Move that value into the first byte of the new stack
	//Jump to the appropriate label for the function
	fputs(	"; Start calling a function \n", file);
	int index = func_table_get_index(ftbl, name);
	//Set the paramters as variables
	size_t num_args = *args;
	char **param_names = func_table_params(ftbl, index);
	fputs(	"; Set the values for the parameters \n", file);
	for(size_t i = 0; i < num_args; i++) {
		emit_var_new(param_names[i]); //Create the variable for the parameter
		emit_var_set(param_names[i], data + i, type + i); //Set the passed value
	}
	fputs("add =1 R2 R2\n", file);
	emit_get_stack_top();
	fprintf(file, 	"gcb R3\n"
					"add =43 R3 R3\n"
					"mov R3 R$0\n"
					"brn =%d\n", func_table_get_label(ftbl, index));
	fputs(	"; Stop calling a function\n", file);
}

static void emit_return_fun(NodeData *data, NodeType *type) {
	emit_var_set("__return", data, type);
	int reg = 1;
	emit_var_get("__return", &reg);
	emit_get_stack_top();
	fputs(	"sub =1 R2 R2\n"
			"gtb R$0\n", file);
}

static void emit_start_main() {
	fputs("lbl =0\n", file);
}

static void emit_end_main() {
	fputs("end\n", file);
}

//TODO: Actual handling of expressions in ifs
static void emit_if_start() {
	stack_push(labels, label); //store what the label was when this if started
	fputs("cmp R0 =0\n", file); //compare the register to 0
	label += 2; //increment the jump label
}

static void emit_if_body_start() {
	int lbl = stack_peek(labels); //find the label of the if that this is the body for
	fprintf(file, "brneq =%d\n", lbl); //If the condition was false, go to the else
}

static void emit_else_body_start() {
	putc('\n', file); //Newline for safety
	int lbl = stack_peek(labels); //find the label for the if this else is for
	fprintf(file, "brn =%d\n", lbl + 1); //Go to the end of the if (only reached if the program did not branch to else)
	fprintf(file, "lbl =%d\n", lbl); //Branch point for the start of the else
}

static void emit_if_end() {
	int lbl = stack_pop(labels); //Pop off this if from the label stack
	putc('\n', file); //newline for safety
	fprintf(file, "lbl =%d\n", lbl + 1); //Branch point for jumping over the else
}

static void emit_while_start() {
	stack_push(labels, label); //store what the label was when this while started
	fprintf(file, "lbl =%d\n", label); //store the label to return to at the end of a loop iteration
	fputs("cmp R0 =0\n", file); //start a comparison
	label += 2; //increment the jump label
}
static void emit_while_body_start() {
	int lbl = stack_peek(labels);
	fprintf(file, "brnne =%d\n", lbl + 1); 
}

static void emit_while_end() {
	int lbl = stack_pop(labels);
	putc('\n', file); //put a newline 
	fprintf(file, "brn =%d\n", lbl); //Go back to the beginning of the loop
	fprintf(file, "lbl =%d\n", lbl + 1); //Marks the end of the loop
}
