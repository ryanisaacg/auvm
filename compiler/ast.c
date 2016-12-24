#include "ast.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

Node *node_new_nil() {
	NodeData nd;
	return node_new(nd, NIL_NODE);
}
Node *node_new_str(char *data, NodeType type) {
	NodeData nd;
	nd.sval = data;
	return node_new(nd, type);
}
Node *node_new_int(int data, NodeType type) {
	NodeData nd;
	nd.ival = data;
	return node_new(nd, type);
}
Node *node_new(NodeData data, NodeType type) {
	Node *new 	= malloc(sizeof(Node));
	new->data 	= data;
	new->type 	= type;
	new->child 	= NULL;
	new->next	= NULL;
	return new;
}

//Walk linked-list of children
void node_add_child(Node *parent, Node *child) {
	if(parent->child == NULL) {
		parent->child = child;
	} else {
		Node *walk = parent->child;
		while(walk->next != NULL) {
			walk = walk->next;
		}
		walk->next = child;
	}
}

static void node_print_indent(Node *root, int indent) {
	for(int i = 0; i < indent; i++) putc(' ', stdout);
	//Extract and print data
	switch(root->type) {
		case WORD_NODE:
			printf("Word: %s\n", root->data.sval);
			break;
		case STRING_NODE:
			printf("String: %s\n", root->data.sval);
			break;
		case NUMBER_NODE:
			printf("Number: %d\n", root->data.ival);
			break;
		case NIL_NODE:
			puts("Nil\n");
			break;
	}
	//Print children indented
	for(Node *child = root->child; child != NULL; child = child->next) {
		node_print_indent(child, indent + 1);
	}
}

void node_print(Node *root) {
	node_print_indent(root, 0);
}

char *node_tostring(Node *node) {
	char *sval;
	switch(node->type) {
	case WORD_NODE:
	case STRING_NODE:
		sval = node->data.sval;
		break;
	case NUMBER_NODE:
		sval = malloc(32);
		sprintf(sval, "%d", node->data.ival);
		break;
	case NIL_NODE:
		sval = "nil";
		break;
	}
	return sval;
}

static inline void node_output_asm(Node *root, FILE *stream, char *command) {
	char *sval = root->data.sval;
	if(strcmp(sval, command) == 0) {
		fprintf(stream, "%s ", command);
		for(Node *child = root->child; child != NULL; child = child->next) {
			fprintf(stream, "%s ", node_tostring(child));
		}
	}
}

#define NODE_MIRROR(str) if(strcmp(sval, str) == 0) { fputs(str, stream); return true; }

static bool node_asm_literal(char *sval, FILE *stream) {
	NODE_MIRROR("mov");
	NODE_MIRROR("add");
	NODE_MIRROR("sub");
	NODE_MIRROR("mul");
	NODE_MIRROR("div");
	NODE_MIRROR("mod");
	NODE_MIRROR("rfi");
	NODE_MIRROR("wto");
	NODE_MIRROR("cmp");
	NODE_MIRROR("brn");
	NODE_MIRROR("pfs");
	NODE_MIRROR("pos");
	NODE_MIRROR("and");
	NODE_MIRROR("ior");
	NODE_MIRROR("xor");
	NODE_MIRROR("rhd");
	NODE_MIRROR("whd");
	NODE_MIRROR("end");
	NODE_MIRROR("lbl");
	NODE_MIRROR("gtb");
	NODE_MIRROR("gcb");
	return false;
}

FunctionTable *functions = NULL;
int label = 1;

static void call_func(char *funcname, FILE *stream) {
	//Get the current amount of stacks
	//Add 1 to it
	//Multiply it by the stack size (1024 in decimal, 400 in hex)
	//Add 1 to it
	//Get the current byte the CPU is looking at
	//Add 36 to the value (24 in hex) to move past the instruction when returning
	//Move that value into the first byte of the new stack
	//Jump to the appropriate label for the function
	fputs("mov %0 R0\n\
add R0 =1 R0\n\
mov R0 %0\n\
mul R0 =1024 R0\n\
add R0 =1 R0\n\
gcb R1\n\
add =43 R1 R1\n\
mov R1 R$0\n", stream);
	fprintf(stream, "brn =%d", func_table_get(functions, funcname));
}

static void func_return(FILE *stream) {
	//Get the current amount of stacks
	//Multiply it by the stack size (1024 in decimal, 400 in hex)
	//Add 1 to it
	//Subtract one from the amount of stacks
	//Store it as the number of stacks
	//Get the byte indicated by the first value in the stack
	//Jump there
	fputs("mov %0 R0\n\
mul R0 =1024 R1\n\
add R1 =1 R1\n\
sub =1 R0 R0\n\
mov R0 %0\n\
gtb R$1", stream);
}

static void node_to_output(Node *root, Table *table, FILE *stream) {
	char *sval 		= root->data.sval;
	int ival		= root->data.ival;
	bool space  	= true;
	bool eval_child = true;
	switch(root->type) {
	case WORD_NODE:
		if(node_asm_literal(sval, stream)) break;
		if(strcmp(sval, "const") == 0) {
			fputc('=', stream);
			space = false;
			break;
		} else if(strcmp(sval, "var-new") == 0) {
			eval_child = false;
			char *varname = root->child->data.sval;
			fputs("mov %0 R0\n\
mul R0 =1024\n\
add R0 =1 R0\n\
add R0 R$0 R1\n", stream);
			Node *value = root->child->next;
			if(value->type == WORD_NODE) {
				int position = table_get(table, value->data.sval);
				fprintf(stream, "add R0 =%d R2\n", position);
				fputs("mov R$2 R$1\n", stream);
			} else if(value->type == NUMBER_NODE) {
				int number = value->data.ival;
				fprintf(stream, "mov =%d R$1\n", number);
			}
			fputs("add R$0 =4 R3\nmov R3 R$0", stream);
			table_add(table, varname);
		} else if(strcmp(sval, "defn") == 0) {
			Node *name_node = root->child;
			Node *body_node = root->child->next->next;
			char *name = name_node->data.sval;
			func_table_add(functions, name, label);
			fprintf(stream, "lbl =%d \n; start function %s\n", label, name);
			label++;
			eval_child = false;
			while(body_node != NULL) {
				node_to_output(body_node, table, stream);
				putc('\n', stream);
				body_node = body_node->next;
			}
			putc('\n', stream);
			func_return(stream);
			fprintf(stream, "\n; return function %s", name);
		} else if(strcmp(sval, "call") == 0) {
			Node *func_name = root->child;
			fprintf(stream, "\n; call function %s\n", func_name->data.sval);
			call_func(func_name->data.sval, stream);
			fprintf(stream, "\n; end function call %s\n", func_name->data.sval);
			eval_child = false;
		} else if(strcmp(sval, "if") == 0) {
			Node *condition_node = root->child;
			Node *if_body = condition_node->next;
			Node *else_body = if_body->next;
			fputs("cmp ", stream);
			node_to_output(condition_node, table, stream);
			fputs("=1\n", stream);
			fprintf(stream, "brnne =%d\n", label);
			node_to_output(if_body, table, stream);
			putc('\n', stream);
			fprintf(stream, "brn =%d\n", label + 1); 
			fprintf(stream, "lbl =%d\n", label);
			node_to_output(else_body, table, stream);
			putc('\n', stream);
			fprintf(stream, "lbl =%d\n", label + 1);
			label += 2;
			eval_child = false;
		} else if(strcmp(sval, "while") == 0) {
			Node *condition_node = root->child;
			Node *if_body = condition_node->next;
			fprintf(stream, "lbl =%d\n", label);
			fputs("cmp ", stream);
			node_to_output(condition_node, table, stream);
			fputs("=1\n", stream);
			fprintf(stream, "brnne =%d\n", label + 1);
			node_to_output(if_body, table, stream);
			putc('\n', stream);
			fprintf(stream, "brn =%d\n", label);
			fprintf(stream, "lbl =%d\n", label + 1); 
			label += 2;
			eval_child = false;
		} else if(strcmp(sval, "true") == 0) {
			fputs("=1 ", stream);
		} else if(strcmp(sval, "false") == 0) {
			fputs("=0 ", stream);
		} else {
			fputs(sval, stream);
		}
		break;
	case NUMBER_NODE:
		fprintf(stream, "%d", ival);
	default:
		break;
	}
	if(space) {
		fputc(' ', stream);
	}
	for(Node *child= root->child; eval_child && child != NULL; child = child->next) {
		node_output(child, stream);
	}
}

void node_output(Node *root, FILE *stream) {
	if(functions == NULL) {
		functions = func_table_new();
	}
	node_to_output(root, table_new(NULL), stream);
}
