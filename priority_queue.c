#include <stdio.h>
#include <stdlib.h>

typedef struct priority_queue {
    int *array;
    int size;
    int capacity;
} priority_queue;

priority_queue *priority_queue_create(int capacity) {
    priority_queue *queue = (priority_queue*)malloc(sizeof(priority_queue));
    queue->size = 0;
    queue->capacity = capacity;
    queue->array = (int*)malloc(capacity*sizeof(int));
    return queue;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void priority_queue_heapify(priority_queue *queue, int i) {
    int largest = i;
    int left = 2*i+1;
    int right = 2*i+2;
    if (left < queue->size && queue->array[left] > queue->array[largest])
        largest = left;

    if (right < queue->size && queue->array[right] > queue->array[largest])
        largest = right;

    if (largest != i) {
        swap(&queue->array[i], &queue->array[largest]);
        priority_queue_heapify(queue, largest);
    }
}

void priority_queue_enqueue(priority_queue *queue, int value) {
    if (queue->size == queue->capacity)
        return;

    // ++queue->size;
    int i = queue->size++; // - 1;
    queue->array[i] = value;

    while (i != 0 && queue->array[(i - 1) / 2] < queue->array[i]) {
        swap(&queue->array[i], &queue->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int priority_queue_dequeue(priority_queue *queue) {
    if (!queue->size)
        return -1;

    int item = queue->array[0];
    queue->array[0] = queue->array[--queue->size];
    priority_queue_heapify(queue, 0);
    return item;
}

int priority_queue_peek(priority_queue *queue) {
    if (!queue->size)
        return -1;

    return queue->array[0];
}

int main() {
    priority_queue *queue = priority_queue_create(10);
    priority_queue_enqueue(queue, 50);
    priority_queue_enqueue(queue, 22);
    priority_queue_enqueue(queue, 43);
    priority_queue_enqueue(queue, 87);
    priority_queue_enqueue(queue, 23);
    priority_queue_enqueue(queue, 9);
    priority_queue_enqueue(queue, 12);
    priority_queue_enqueue(queue, 7);
    printf("%d\n", priority_queue_dequeue(queue));
    printf("%d\n", priority_queue_dequeue(queue));
    printf("%d\n", priority_queue_dequeue(queue));
    printf("%d\n", priority_queue_dequeue(queue));
}