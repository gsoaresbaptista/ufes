#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

// Create a stack with initial capacity
stack* stack_create(int capacity) {
    stack* s = (stack*)malloc(sizeof(stack));
    s->items = (int*)malloc(capacity * sizeof(int));
    s->top = -1;
    s->capacity = capacity;
    return s;
}

// Push an element onto the stack
void stack_push(stack* s, int value) {
    if (s->top == s->capacity - 1) {
        s->capacity *= 2;  // Double the capacity if needed
        s->items = (int*)realloc(s->items, s->capacity * sizeof(int));
    }
    s->items[++s->top] = value;
}

// Pop an element from the stack
int stack_pop(stack* s) {
    if (s->top == -1) {
        return -1;
    }
    return s->items[s->top--];
}

// Free the stack
void stack_free(stack* s) {
    free(s->items);
    free(s);
}
