#include <stdio.h>
#include <stdlib.h>

typedef struct dlinked_list_node {
    void* data;
    struct dlinked_list_node *prev, *next;
} dlinked_list_node;

typedef struct LinkedList {
    dlinked_list_node *head, *tail;
    size_t size;
} dlinked_list;

typedef void (*list_fn)(void *);

typedef int (*list_cmp)(const void *data, const void *key);

dlinked_list *dlinked_list_create() {
    dlinked_list *list = malloc(sizeof *list);
    if (!list) return NULL;
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void dlinked_list_add_front(dlinked_list *list, void *data) {
    dlinked_list_node *node = malloc(sizeof *node);
    if (!node) return;

    node->data = data;
    node->prev = NULL;
    node->next = list->head;

    if(list->head)
        list->head->prev = node;
    else
        list->tail = node;

    list->head = node;
    ++list->size;
}

void dlinked_list_add_end(dlinked_list *list, void *data) {
    dlinked_list_node *node = malloc(sizeof *node);
    if (!node) return;
    
    node->data = data;
    node->next = NULL;
    node->prev = list->tail;

    if(list->tail)
        list->tail->next = node;
    else
        list->head = node;
    list->tail = node;
    ++list->size;
}


void dlinked_list_foreach(const dlinked_list *list, list_fn fn) {
    dlinked_list_node *cur = list->head;
    while(cur != NULL) {
        fn(cur->data);
        cur = cur->next;
    }
}

void dlinked_list_clear(dlinked_list *list, list_fn free_data) {
    dlinked_list_node *cur = list->head;
    while (cur) {
        dlinked_list_node *next = cur->next;
        if (free_data)
            free_data(cur->data);
        free(cur);
        cur = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void dlinked_list_destroy(dlinked_list *list, list_fn free_data) {
    dlinked_list_clear(list, free_data);
    free(list);
}

void *dlinked_list_remove_at(dlinked_list *list, size_t index) {
    if(!list || index >= list->size)
        return NULL;
    dlinked_list_node *cur;
    void *data;

    if(index < list->size / 2) {
        cur = list->head;
        for(size_t i=0;i<index;++i)
            cur = cur->next;
    } else {
        cur = list->tail;
        for(size_t i=list->size-1;i>index;--i)
            cur = cur->next;
    }

    if(cur->prev)
        cur->prev->next = cur->next;
    else
        list->head = cur->next;
    
    if(cur->next)
        cur->next->prev = cur->prev;
    else
        list->tail = cur->prev;

    data = cur->data;
    free(cur);
    --list->size;
    return data;
}

int dlinked_list_remove(dlinked_list *list, const void *key, list_cmp cmp, list_fn free_data) {
    if (!list || !cmp) return 0;

    dlinked_list_node *cur = list->head;

    while (cur) {
        if (cmp(cur->data, key)) {
            if (cur->prev)
                cur->prev->next = cur->next;
            else
                list->head = cur->next;

            if(cur->next)
                cur->next->prev = cur->prev;
            else
                list->tail = cur->prev;

            if (free_data)
                free_data(cur->data);

            free(cur);
            --list->size;
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

void print_int(void *data) {
    printf("%d, ", *(int *)data);
}

void free_int(void *data) {
    free(data);
}

int int_cmp(const void *data, const void *key) {
    return *(const int *)data == *(const int *)key;
}

int main() {
    dlinked_list *list = dlinked_list_create();
    for(int i=0;i<10;++i) {
        int *x = malloc(sizeof(int));
        *x = i;
        dlinked_list_add_end(list, x);
    }

    int *removed = dlinked_list_remove_at(list, 3);
    if (removed) {
        printf("Removed: %d\n", *removed);
        free(removed);
    }

    int key = 8;
    dlinked_list_remove(list, &key, int_cmp, free_int);

    printf("[");
    dlinked_list_foreach(list, print_int);
    printf("]\n");
    dlinked_list_destroy(list, free_int);
    return 0;
}