#include <stdio.h>
#include <stdlib.h>

typedef struct max_heap {
    int *array;
    int size;
    int capacity;
} max_heap;

max_heap *max_heap_create(int capacity) {
    max_heap *heap = (max_heap*)malloc(sizeof(max_heap));
    heap->size = 0;
    heap->capacity = capacity;
    heap->array = (int*)malloc(capacity*sizeof(int));
    return heap;
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void max_heap_heapify(max_heap *heap, int i) {
    int largest = i;
    int left = 2*i+1;
    int right = 2*i+2;
    if (left < heap->size && heap->array[left] > heap->array[largest])
        largest = left;

    if (right < heap->size && heap->array[right] > heap->array[largest])
        largest = right;

    if (largest != i) {
        swap(&heap->array[i], &heap->array[largest]);
        max_heap_heapify(heap, largest);
    }
}

void max_heap_insert(max_heap *heap, int value) {
    if (heap->size == heap->capacity)
        return;

    ++heap->size;
    int i = heap->size - 1;
    heap->array[i] = value;

    while (i != 0 && heap->array[(i - 1) / 2] < heap->array[i]) {
        swap(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

void max_heap_delete(max_heap *heap, int index) {
    if (index >= heap->size)
        return;

    if (index == heap->size - 1) {
        --heap->size;
        return;
    }

    heap->array[index] = heap->array[heap->size - 1];
    --heap->size;
    max_heap_heapify(heap, index);
}

static void print_heap_node(max_heap *heap, int index, const char *prefix, int isTail, const char *relation) {
    if (index >= heap->size)
        return;

    printf("%s%s%s%d\n", prefix, isTail ? " └── " : " ├── ", relation, heap->array[index]);

    char newPrefix[256];
    sprintf(newPrefix, "%s%s", prefix, isTail ? "     " : " │   ");

    int left  = 2 * index + 1;
    int right = 2 * index + 2;

    int children[2];
    int count = 0;

    if (left < heap->size)  children[count++] = left;
    if (right < heap->size) children[count++] = right;

    for (int i = 0; i < count - 1; i++) {
        const char *rel = (children[i] == left) ? "(L) " : "(R) ";
        print_heap_node(heap, children[i], newPrefix, 0, rel);
    }

    if (count > 0) {
        int last = children[count - 1];
        const char *rel = (last == left) ? "(L) " : "(R) ";
        print_heap_node(heap, last, newPrefix, 1, rel);
    }
}

int main() {
    max_heap *heap = max_heap_create(10);
    max_heap_insert(heap, 1);
    max_heap_insert(heap, 2);
    max_heap_insert(heap, 3);
    max_heap_insert(heap, 4);
    max_heap_insert(heap, 5);
    max_heap_insert(heap, 9);
    max_heap_insert(heap, 12);
    max_heap_insert(heap, 7);
    print_heap_node(heap, 0, "", 1, "(T)");
    max_heap_insert(heap, 16);
    // max_heap_delete(heap, 5);
    print_heap_node(heap, 0, "", 1, "(T)");
    //print_heap(max_heap_search(heap, 9), "", 1, "(7) ");
}