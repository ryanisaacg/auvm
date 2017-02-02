#include "stack.h"
#include <stdlib.h>

Stack stack_new() {
	return (Stack) {
		.buffer = malloc(sizeof(int) * 1024),
		.length = 0,
		.capacity = 1024
	};
}

bool stack_empty(Stack *stack) {
	return stack->length <= 0;
}

void stack_push(Stack *stack, int x) {
	if(stack->length * 2 < stack->capacity) {
		stack->buffer[stack->length] = x;
		stack->length++;
	} else {
		stack->capacity *= 2;
		stack->buffer = realloc(stack->buffer, stack->capacity);
		stack_push(stack, x);
	}
}

int stack_peek(Stack *stack) {
	return stack->buffer[stack->length - 1];
}

int stack_pop(Stack *stack) {
	stack->length--;
	return stack->buffer[stack->length];
}
