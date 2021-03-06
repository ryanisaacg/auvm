#include "table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Table *table_new(Table *parent) {
	Table *new = malloc(sizeof(Table));
	new->parent = parent;
	new->length = 0;
	if(parent == NULL) {
		new->stack_level = 0;
	} else {
		new->stack_level = parent->stack_level + 1;
	}
	return new;
}
void table_add(Table *tbl, char *name) {
	tbl->names[tbl->length] = name;
	tbl->length++;
	if(tbl->length > 1024) {
		fputs("Added too many entries to a single table.", stderr);
	}
}


int table_get(Table *tbl, char *name, int *out_stack_level) {
	for(size_t i = 0; i < tbl->length; i++) {
		if(strcmp(tbl->names[i], name) == 0) {
			if(out_stack_level != NULL) {
				*out_stack_level = tbl->stack_level;
			}
			return i;
		}
	}
	if(tbl->parent != NULL) {
		return table_get(tbl->parent, name, out_stack_level);
	} else {
		if(out_stack_level != NULL) {
			*out_stack_level = tbl->stack_level;
		}
		return -1;
	}
}

FunctionTable *func_table_new() {
	FunctionTable *ft = malloc(sizeof(FunctionTable));
	ft->length = 0;
	return ft;
}
void func_table_add(FunctionTable *ft, char *name, char **params, int label) {
	ft->names[ft->length] = name;
	ft->labels[ft->length] = label;
	ft->param[ft->length] = params;
	ft->length++;
	if(ft->length > 1024) {
		fputs("Added too many entries to a function table.", stderr);
	}
}
int func_table_get_index(FunctionTable *tbl, char *name) {
	for(size_t i = 0; i < tbl->length; i++) {
		if(strcmp(tbl->names[i], name) == 0) {
			return i;
		}
	}
	return -1;
}

int func_table_get_label(FunctionTable *tbl, int index) {
	return tbl->labels[index];
}

char **func_table_params(FunctionTable *tbl, int index) {
	return tbl->param[index];
}
