#include "ast.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define NODE_MIRROR(str) if(strcmp(sval, str) == 0) { fputs(str, stream); fputc(' ', stream); break; }

void node_output(Node *root, FILE *stream) {
	char *sval 	= root->data.sval;
	int ival	= root->data.ival;
	bool space  = true;
	switch(root->type) {
	case WORD_NODE:
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
		if(strcmp(sval, "const") == 0) {
			fputc('=', stream);
			space = false;
			break;
		}
		fputs(sval, stream);
		break;
	case NUMBER_NODE:
		fprintf(stream, "%d", ival);
	default:
		break;
	}
	if(space) {
		fputc(' ', stream);
	}
	for(Node *child = root->child; child != NULL; child = child->next) {
		node_output(child, stream);
	}
}
