#include <stdio.h>
#include <stdlib.h>

typedef struct slinked_list_node {
    void* data;
    struct slinked_list_node* next;
} slinked_list_node;

typedef struct LinkedList {
    slinked_list_node *head;
    size_t size;
} slinked_list;

typedef void (*list_fn)(void *);

typedef int (*list_cmp)(const void *data, const void *key);

slinked_list *slinked_list_create() {
    slinked_list *list = malloc(sizeof *list);
    if (!list) return NULL;
    list->head = NULL;
    list->size = 0;
    return list;
}

void slinked_list_add_front(slinked_list *list, void *data) {
    slinked_list_node *node = malloc(sizeof *node);
    if (!node) return;
    node->data = data;
    node->next = list->head;
    list->head = node;
    ++list->size;
}


void slinked_list_foreach(const slinked_list *list, list_fn fn) {
    /*for (slinked_list_node *cur = list->head; cur; cur = cur->next) {
        fn(cur->data);
    }*/
    slinked_list_node *cur = list->head;
    while(cur != NULL) {
        fn(cur->data);
        cur = cur->next;
    }
}

void slinked_list_clear(slinked_list *list, list_fn free_data) {
    slinked_list_node *cur = list->head;
    while (cur) {
        slinked_list_node *next = cur->next;
        if (free_data)
            free_data(cur->data);
        free(cur);
        cur = next;
    }
    list->head = NULL;
    list->size = 0;
}

void slinked_list_destroy(slinked_list *list, list_fn free_data) {
    slinked_list_clear(list, free_data);
    free(list);
}

void *slinked_list_remove_at(slinked_list *list, size_t index) {
    if(!list || index >= list->size)
        return NULL;
    slinked_list_node *cur;
    void *data;

    if(index == 0) {
        cur = list->head;
        list->head = cur->next;
    } else {
        slinked_list_node *prev = list->head;
        for(size_t i = 0; i < index - 1; ++i)
            prev = prev->next;
        cur = prev->next;
        prev->next = cur->next;
    }

    data = cur->data;
    free(cur);
    --list->size;

    return data;
}

int slinked_list_remove(slinked_list *list, const void *key, list_cmp cmp, list_fn free_data) {
    if (!list || !cmp) return 0;

    slinked_list_node *cur = list->head;
    slinked_list_node *prev = NULL;
    while (cur) {
        if (cmp(cur->data, key)) {
            if (prev)
                prev->next = cur->next;
            else
                list->head = cur->next;

            if (free_data)
                free_data(cur->data);

            free(cur);
            --list->size;
            return 1;
        }
        prev = cur;
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
    slinked_list *list = slinked_list_create();
    for(int i=0;i<10;++i) {
        int *x = malloc(sizeof(int));
        *x = i;
        slinked_list_add_front(list, x);
    }

    int *removed = slinked_list_remove_at(list, 3);
    if (removed) {
        printf("Removed: %d\n", *removed);
        free(removed);
    }

    int key = 8;
    slinked_list_remove(list, &key, int_cmp, free_int);

    printf("[");
    slinked_list_foreach(list, print_int);
    printf("]\n");
    slinked_list_destroy(list, free_int);
    return 0;
}