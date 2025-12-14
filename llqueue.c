#include <stdio.h>
#include <stdlib.h>

typedef struct llnode {
    int data;
    struct llnode* next;
} llnode;

typedef struct llqueue {
    llnode *front;
    llnode *back;
    int size;
} llqueue;

llqueue *llqueue_create() {
    llqueue* q = (llqueue*)malloc(sizeof(llqueue));
    q->front = NULL;
    q->back = NULL;
    q->size = 0;
    return q;
}

void llqueue_enqueue(llqueue *q, int data) {
    llnode *node = malloc(sizeof *node);
    node->data = data;
    node->next = NULL;
    if(q->front == NULL) {
        q->front = q->back = node;
    } else {
        q->back->next = node;
        q->back = node;
    }
    ++q->size;
}

int llqueue_dequeue(llqueue *q) {
    if(q->front == NULL)
        return -1;
    llnode *cur = q->front;
    int data = cur->data;
    q->front = q->front->next;
    if(q->front == NULL)
        q->back = NULL;
    free(cur);
    --q->size;
    return data;
}

int llqueue_peek(llqueue *q) {
    if(q->front == NULL)
        return -1;
    return q->front->data;
}

int main() {
    llqueue *queue = llqueue_create();

    llqueue_enqueue(queue, 3);
    llqueue_enqueue(queue, 7);
    llqueue_enqueue(queue, 1);

    printf("%d\n", llqueue_peek(queue));
    llqueue_dequeue(queue);
    printf("%d\n", llqueue_dequeue(queue));

    printf("%d\n", llqueue_peek(queue));
    llqueue_enqueue(queue, 9);
    printf("%d\n", llqueue_peek(queue));
    llqueue_dequeue(queue);
    printf("%d\n", llqueue_peek(queue));
}