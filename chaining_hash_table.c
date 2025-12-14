#include <stdio.h>
#include <stdlib.h>

typedef struct chaining_hash_node {
    int key;
    char value;
    struct chaining_hash_node *next;
} chaining_hash_node;

typedef struct chaining_hash_table {
    chaining_hash_node **arr;
    size_t size;
} chaining_hash_table;

static int chaining_hash(chaining_hash_table *ht, int key) {
    return key % ht->size;
}

chaining_hash_table *chaining_hash_table_create(size_t size) {
    chaining_hash_table *ht = malloc(sizeof *ht);
    if(!ht) return NULL;
    ht->arr = calloc(size, sizeof *ht->arr);
    ht->size = size;
    if(!ht->arr) {
        free(ht);
        return NULL;
    }
    return ht;
}

void chaining_hash_table_insert(chaining_hash_table *table, int key, char value) {
    int index = chaining_hash(table, key);

    chaining_hash_node *current = table->arr[index];

    while (current) {
        if (current->key == key) {
            current->value = value;
            return;
        }
        current = current->next;
    }

    chaining_hash_node *node = malloc(sizeof(chaining_hash_node));
    node->key = key;
    node->value = value;
    node->next = table->arr[index];
    table->arr[index] = node;
}

char chaining_hash_table_get(chaining_hash_table *table, int key) {
    int index = chaining_hash(table, key);

    chaining_hash_node *current = table->arr[index];
    while (current) {
        if (current->key == key)
            return current->value;
        current = current->next;
    }
    return -1;
}

void chaining_hash_table_free(chaining_hash_table *table) {
    for (int i = 0; i < table->size; i++) {
        chaining_hash_node *current = table->arr[i];
        while (current) {
            chaining_hash_node *next = current->next;
            free(current);
            current = next;
        }
    }
    free(table->arr);
    free(table);
}

int main() {
    chaining_hash_table *table = chaining_hash_table_create(16);

    chaining_hash_table_insert(table, 15, 'A');
    chaining_hash_table_insert(table, 27, 'B');
    chaining_hash_table_insert(table, 37, 'C');
    chaining_hash_table_insert(table, 44, 'D');
    chaining_hash_table_insert(table, 24, 'E');
    chaining_hash_table_insert(table, 17, 'F');

    printf("15 → %c\n", chaining_hash_table_get(table, 15));
    printf("27 → %c\n", chaining_hash_table_get(table, 27));
    printf("37 → %c\n", chaining_hash_table_get(table, 37));
    printf("44 → %c\n", chaining_hash_table_get(table, 44));
    printf("24 → %c\n", chaining_hash_table_get(table, 24));
    printf("17 → %c\n", chaining_hash_table_get(table, 17));
}
