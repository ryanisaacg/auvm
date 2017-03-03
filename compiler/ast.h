#pragma once

#include <stdio.h>

struct Node;
typedef struct Node Node;

typedef union {
	char *sval;
	int ival;
} NodeData;

typedef enum { WORD_NODE, STRING_NODE, NUMBER_NODE, NIL_NODE, ROOT_NODE} NodeType;

struct Node {
	NodeData data;
	NodeType type;
	Node *child, *next;
};
Node *node_new_root();
Node *node_new_nil();
Node *node_new_str(char *data, NodeType type);
Node *node_new_int(int data, NodeType type);
Node *node_new(NodeData data, NodeType type);
void node_add_child(Node *parent, Node *child);
void node_print(Node *root);
void node_output(Node *root, FILE *stream);
char *node_tostring(Node *node);
