#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_STACK 100

typedef struct llnode {
    int data;
    struct llnode* next;
} llnode;

typedef struct llstack {
    llnode *head;
    size_t size;
} llstack;

void llstack_initialize(llstack *stack) {
    stack->head = NULL;  
}

llstack *llstack_create() {
    llstack *stack = malloc(sizeof *stack);
    if (!stack) return NULL;
    stack->head = NULL;  
    return stack;
}

bool llstack_is_empty(llstack *stack) {
    return stack->head == NULL;
}

bool llstack_is_full(llstack *stack) {
    return stack->size == MAX_STACK-1;
}

void llstack_push(llstack *stack, int value) {
    if(llstack_is_full(stack))
        return;
    llnode *node = malloc(sizeof *node);
    node->data = value;
    node->next = stack->head;
    stack->head = node;
    ++stack->size;
}

int llstack_pop(llstack *stack) {
    if(llstack_is_empty(stack))
        return -1;
    llnode *cur = stack->head;
    int data = cur->data;
    stack->head = cur->next;
    free(cur);
    return data;
}

int llstack_peek(llstack *stack) {
    if(llstack_is_empty(stack))
        return -1;
    return stack->head->data;
}

int main() {
    llstack *stack = llstack_create();

    llstack_push(stack, 3);
    llstack_push(stack, 7);
    llstack_push(stack, 1);

    printf("%d\n", llstack_peek(stack));
    llstack_pop(stack);
    printf("%d\n", llstack_pop(stack));

    printf("%d\n", llstack_peek(stack));
    llstack_push(stack, 9);
    printf("%d\n", llstack_peek(stack));
}
