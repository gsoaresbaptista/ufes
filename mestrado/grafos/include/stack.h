#ifndef STACK_H
#define STACK_H

typedef struct stack {
    int* items;
    int top;
    int capacity;
} stack;

/**
 * @brief Creates a stack with a given initial capacity.
 * 
 * @param capacity Initial capacity of the stack.
 * @return Pointer to the newly created stack.
 * 
 * @complexity O(1) for creation, O(N) if resizing is needed later.
 */
stack* stack_create(int capacity);

/**
 * @brief Pushes an item onto the stack.
 * 
 * @param s Pointer to the stack.
 * @param value The value to push onto the stack.
 * 
 * @complexity O(1) amortized for push.
 */
void stack_push(stack* s, int value);

/**
 * @brief Pops an item from the stack.
 * 
 * @param s Pointer to the stack.
 * @return The popped value from the stack.
 * 
 * @complexity O(1) for pop.
 */
int stack_pop(stack* s);

/**
 * @brief Frees the memory allocated for the stack.
 * 
 * @param s Pointer to the stack.
 * 
 * @complexity O(1) for free.
 */
void stack_free(stack* s);

#endif
