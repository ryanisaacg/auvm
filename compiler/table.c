#include "table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Table *table_new(Table *parent) {
	Table *new = malloc(sizeof(Table));
	new->parent = parent;
	new->length = 0;
	return new;
}
void table_add(Table *tbl, char *name) {
	tbl->names[tbl->length] = name;
	tbl->length++;
	if(tbl->length > 1024) {
		fputs("Added too many entries to a single table.", stderr);
	}
}
int table_get(Table *tbl, char *name) {
	for(size_t i = 0; i < tbl->length; i++) {
		if(strcmp(tbl->names[i], name) == 0) {
			return i;
		}
	}
	if(tbl->parent != NULL) {
		return table_get(tbl, name);
	} else {
		return -1;
	}
}

FunctionTable *func_table_new() {
	FunctionTable *ft = malloc(sizeof(FunctionTable));
	ft->length = 0;
	return ft;
}
void func_table_add(FunctionTable *ft, char *name, int label) {
	ft->names[ft->length] = name;
	ft->labels[ft->length] = label;
	ft->length++;
	if(ft->length > 1024) {
		fputs("Added too many entries to a function table.", stderr);
	}
}
int func_table_get(FunctionTable *tbl, char *name) {
	for(size_t i = 0; i < tbl->length; i++) {
		if(strcmp(tbl->names[i], name) == 0) {
			return tbl->labels[i];
		}
	}
	return -1;
}
