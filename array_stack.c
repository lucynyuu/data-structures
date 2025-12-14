#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_STACK 100

typedef struct {
    int arr[MAX_STACK];
    int top;
} astack;

void astack_initialize(astack *stack) {
    stack->top = -1;  
}

astack *astack_create() {
    astack *stack = malloc(sizeof *stack);
    if (!stack) return NULL;
    stack->top = -1;  
    return stack;
}

bool astack_is_empty(astack *stack) {
    return stack->top == -1;
}

bool astack_is_full(astack *stack) {
    return stack->top == MAX_STACK-1;
}

void astack_push(astack *stack, int value) {
    if(astack_is_full(stack))
        return;
    stack->arr[++stack->top] = value;
}

int astack_pop(astack *stack) {
    if(astack_is_empty(stack))
        return -1;
    return stack->arr[stack->top--];
}

int astack_peek(astack *stack) {
    if(astack_is_empty(stack))
        return -1;
    return stack->arr[stack->top];
}

int main() {
    astack *stack = astack_create();

    astack_push(stack, 3);
    astack_push(stack, 7);
    astack_push(stack, 1);

    printf("%d\n", astack_peek(stack));
    astack_pop(stack);
    printf("%d\n", astack_pop(stack));

    printf("%d\n", astack_peek(stack));
    astack_push(stack, 9);
    printf("%d\n", astack_peek(stack));
}
