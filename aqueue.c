#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_QUEUE 100

typedef struct {
    int arr[MAX_QUEUE];
    int front;
    size_t size;
} aqueue;

void aqueue_initialize(aqueue *queue) {
    queue->front = 0;
    queue->size = 0;
}

aqueue *aqueue_create() {
    aqueue *queue = malloc(sizeof *queue);
    if (!queue) return NULL;
    queue->front = 0;
    queue->size = 0;
    return queue;
}

bool aqueue_is_empty(aqueue *q) {
    return q->size==0;
}

void aqueue_enqueue(aqueue *q, int value) {
    if(q->size == MAX_QUEUE)
        return;
    int back = (q->front + q->size)%MAX_QUEUE;
    q->arr[back] = value;
    ++q->size;
}

int aqueue_dequeue(aqueue *q) {
    if(aqueue_is_empty(q))
        return -1;
    int res = q->arr[q->front];
    q->front = (q->front+1)%MAX_QUEUE;
    --q->size;
    return res;
}

int aqueue_peek(aqueue *q) {
    if(aqueue_is_empty(q))
        return -1;
    return q->arr[q->front];
}

int main() {
    aqueue *queue = aqueue_create();

    aqueue_enqueue(queue, 3);
    aqueue_enqueue(queue, 7);
    aqueue_enqueue(queue, 1);

    printf("%d\n", aqueue_peek(queue));
    aqueue_dequeue(queue);
    printf("%d\n", aqueue_dequeue(queue));

    printf("%d\n", aqueue_peek(queue));
    aqueue_enqueue(queue, 9);
    printf("%d\n", aqueue_peek(queue));
    aqueue_dequeue(queue);
    printf("%d\n", aqueue_peek(queue));
}