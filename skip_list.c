#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define MAX_LEVEL 16
#define P 0.5

typedef struct skip_list_node {
    int key;
    struct skip_list_node **forward;
} skip_list_node;

typedef struct skip_list {
    int level;
    skip_list_node *header;
} skip_list;

int skip_list_random_level() {
    int level = 0;
    while (((double)rand() / RAND_MAX) < P && level < MAX_LEVEL - 1)
        ++level;
    return level;
}

skip_list_node *skip_list_create_node(int level, int key) {
    skip_list_node *node = malloc(sizeof(skip_list_node));
    node->key = key;
    node->forward = malloc(sizeof(skip_list_node*) * (level+1));

    for (int i = 0; i <= level; i++)
        node->forward[i] = NULL;

    return node;
}

skip_list *skip_list_create() {
    skip_list *list = malloc(sizeof(skip_list));
    list->level = 0;
    list->header = skip_list_create_node(MAX_LEVEL - 1, INT_MIN);
    return list;
}

skip_list_node *skip_list_search(skip_list *list, int key) {
    skip_list_node *current = list->header;

    for (int i = list->level; i >= 0; i--)
        while (current->forward[i] && current->forward[i]->key < key)
            current = current->forward[i];

    current = current->forward[0];

    if (current && current->key == key)
        return current;

    return NULL;
}

void skip_list_insert(skip_list *list, int key) {
    skip_list_node *update[MAX_LEVEL];
    skip_list_node *current = list->header;

    for (int i = list->level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->key < key)
            current = current->forward[i];
        update[i] = current;
    }

    current = current->forward[0];

    if (current == NULL || current->key != key) {
        int lvl = skip_list_random_level();

        if (lvl > list->level) {
            for (int i = list->level + 1; i <= lvl; i++)
                update[i] = list->header;
            list->level = lvl;
        }

        skip_list_node *new_node = skip_list_create_node(lvl, key);

        for (int i = 0; i <= lvl; i++) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }
    }
}

void skip_list_print(skip_list *list) {
    for (int i = list->level; i >= 0; i--) {
        skip_list_node *node = list->header->forward[i];
        printf("Level %d: ", i);
        while (node) {
            printf("%d ", node->key);
            node = node->forward[i];
        }
        printf("\n");
    }
}

int main() {
    srand((unsigned)time(NULL));

    skip_list *list = skip_list_create();

    skip_list_insert(list, 3);
    skip_list_insert(list, 6);
    skip_list_insert(list, 7);
    skip_list_insert(list, 9);
    skip_list_insert(list, 12);
    skip_list_insert(list, 19);

    skip_list_print(list);

    skip_list_node *found = skip_list_search(list, 7);
    if (found)
        printf("Found %d\n", found->key);
    else
        printf("Not found\n");

    return 0;
}