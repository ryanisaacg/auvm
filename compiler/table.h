#pragma once

#include <stddef.h>

struct Table;
typedef struct Table Table;

struct Table {
	char *names[1024];
	int values[1024];
	size_t length;
	Table *parent;
};

Table *table_new(Table *parent);
void table_add(Table *tbl, char *name);
int table_get(Table *tbl, char *name);

struct FunctionTable;
typedef struct FunctionTable FunctionTable;

struct FunctionTable {
	char *names[1024];
	int labels[1024];
	size_t length;
};
FunctionTable *func_table_new();
void func_table_add(FunctionTable *tbl, char *name, int label);
int func_table_get(FunctionTable *tbl, char *name);
