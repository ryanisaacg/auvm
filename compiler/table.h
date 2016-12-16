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
void table_add(Table *tbl, char *name, int value);
int *table_get(Table *tbl, char *name);
