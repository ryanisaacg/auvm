#pragma once

/**
 * STACK
 *
 * A LIFO (last-in, first-out) array-backed structure
 * Stores ints
 */

#include <stdbool.h>
#include <stddef.h>

typedef struct Stack {
	int *buffer;
	size_t length, capacity;
} Stack;

Stack stack_new();
void stack_push(Stack *stack, int x);
bool stack_empty(Stack *stack);
int stack_peek(Stack *stack);
int stack_pop(Stack *stack);
