#include "ast.h"

#include <stdio.h>
#include <stdlib.h>

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
			printf("Number: %s\n", root->data.ival);
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
