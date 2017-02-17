#include "ast.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ir.h"
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

static void node_to_output(Node *root) {
	char *sval = root->data.sval;
	switch(root->type) {
	case WORD_NODE:
		if(strcmp(sval, "var-new") == 0) {
			ir_new_var(root->child->data.sval);
			ir_set_var(root->child->data.sval, root->child->next->data, root->child->next->type);
		} else if(strcmp(sval, "var-get") == 0) {
			ir_get_var(root->child->data.sval, root->child->next->data.ival);
		} else if(strcmp(sval, "var-set") == 0) {
			ir_set_var(root->child->data.sval, root->child->next->data, root->child->next->type);
		} else if(strcmp(sval, "defn") == 0) {
			Node *name_node = root->child;
			Node *body_node = root->child->next->next;
			char *name = name_node->data.sval;
			Node *parameters = name_node->next;
			char **param_list;
			if(parameters->type != NIL_NODE) {
				char **param_list = malloc(sizeof(char*) * 8);
				size_t length = 1, capacity = 8;
				Node *current = name_node->next;
				param_list[0] = current->data.sval;
				current = current->child;
				while(current != NULL) {
					if(length * 2 >= capacity) {
						capacity *= 2;
						param_list = realloc(param_list, sizeof(char*) * capacity);
					}
					param_list[length] = current->data.sval;
					length++;
					current = current->next;
				}
				param_list[length] = NULL;
			} else {
				param_list = malloc(sizeof(char*));
				*param_list = NULL;
			}
			ir_start_fun(name, param_list);
			while(body_node != NULL) {
				node_to_output(body_node);
			}
			ir_end_fun();
		} else if(strcmp(sval, "call") == 0) {
			NodeData *data = malloc(sizeof(NodeData) * 2);
			NodeType *type = malloc(sizeof(NodeType) * 2);
			size_t capacity = 2;
			size_t length = 0;
			Node *arg_node = root->child->next;
			while(arg_node != NULL) {
				if(length * 2 < capacity) {
					data[length] = arg_node->data;
					type[length] = arg_node->type;
					length++;
					arg_node = arg_node->next;
				} else {
					capacity *= 2;
					data = realloc(data, sizeof(NodeData) * capacity);
					type = realloc(data, sizeof(NodeType) * capacity);
				}
			}
			ir_call_fun(root->child->data.sval, data, type, length);
		} else if(strcmp(sval, "if") == 0) {
			//Get the value of the variable being checked against
			ir_get_var(root->child->data.sval, 0);
			ir_if_start();
			ir_if_body_start();
			node_to_output(root->child->next);
			ir_else_body_start();
			node_to_output(root->child->next->next);
			ir_if_end();
		} else if(strcmp(sval, "while") == 0) {
			//Get the value of the variable being checked against
			ir_get_var(root->child->data.sval, 0);
			ir_while_start();
			ir_while_body_start();
			node_to_output(root->child->next);
			ir_while_end();
		} else if(strcmp(sval, "main") == 0) {
			ir_start_main();
			Node *current = root->child;
			while(current != NULL) {
				node_to_output(current);
				current = current->next;
			}
			ir_end_main();
		} else if(strcmp(sval, "asm") == 0) {
			ir_inline(root->child->data.sval);
		} else if(strcmp(sval, "true") == 0) {
			ir_inline("=1 ");
		} else if(strcmp(sval, "false") == 0) {
			ir_inline("=0 ");
		}
		break;
	default:
		break;
	}
}

void node_output(Node *root, FILE *stream) {
	ir_start_file();
	node_to_output(root);
	ir_end_file(stream);
}
