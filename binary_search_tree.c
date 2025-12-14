#include <stdio.h>
#include <stdlib.h>

typedef struct binary_tree_node {
    int data;
    struct binary_tree_node *left;
    struct binary_tree_node *right;
} binary_tree_node;

typedef struct binary_tree {
    binary_tree_node *root;
    size_t size;
} binary_tree;

binary_tree* binary_tree_create() {
    binary_tree* tree = (binary_tree*)malloc(sizeof(binary_tree));
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

binary_tree_node* binary_tree_create_node(int data, binary_tree_node *left, binary_tree_node *right) {
    binary_tree_node* node = (binary_tree_node*)malloc(sizeof(binary_tree_node));
    node->data = data;
    node->left = left;
    node->right = right;
    return node;
}

void binary_tree_insert(binary_tree *tree, int data) {
    binary_tree_node *node = binary_tree_create_node(data, NULL, NULL);
    if(tree->root == NULL) {
        tree->root = node;
        tree->size = 1;
        return;
    }

    binary_tree_node *cur = tree->root;
    while(1) {
        if(data < cur->data) {
            if(cur->left != NULL)
                cur = cur->left;
            else {
                cur->left = node;
                ++tree->size;
                return;
            }
        } else {
            if(cur->right != NULL)
                cur = cur->right;
            else {
                cur->right = node;
                ++tree->size;
                return;
            }
        }
    }
}

void binary_tree_delete(binary_tree *tree, int data) {
    binary_tree_node *parent = NULL;
    binary_tree_node *cur = tree->root;
    while(cur != NULL && cur->data != data) {
        parent = cur;
        if(data < cur->data)
            cur = cur->left;
        else
            cur = cur->right;
    }

    if(cur == NULL)
        return;

    if(cur->left != NULL && cur->right != NULL) {
        binary_tree_node *succ_parent = cur;
        binary_tree_node *succ = cur->right;
        while(succ->left) {
            succ_parent = succ;
            succ = succ->left;
        }
        cur->data = succ->data;
        parent = succ_parent;
        cur = succ;
    }

    binary_tree_node *child = cur->left?cur->left:cur->right;

    if(parent == NULL)
        tree->root = child;
    else if(parent->left == cur)
        parent->left = child;
    else
        parent->right = child;
    free(cur);
    --tree->size;
}

binary_tree_node *binary_tree_search(binary_tree *tree, int data) {
    if(tree->root == NULL)
        return NULL;
    binary_tree_node *cur = tree->root;
    while(cur != NULL) {
        if(cur->data == data)
            return cur;
        else if(cur->data > data)
            cur = cur->left;
        else if(cur->data < data)
            cur = cur->right;
    }
}

static void print_tree_node(binary_tree_node *node, const char *prefix, int isTail, const char *relation) {
    if (!node) 
        return;

    printf("%s%s%s[%d]\n", prefix, isTail ? " └── " : " ├── ", relation, node->data);

    const char *newPrefix = isTail ? "     " : " │   ";

    binary_tree_node *children[2];
    int count = 0;

    if (node->left) children[count++] = node->left;
    if (node->right) children[count++] = node->right;

    for (int i = 0; i < count - 1; i++) {
        const char *rel = (children[i] == node->left) ? "(L) " : "(R) ";
        char updatedPrefix[100];
        sprintf(updatedPrefix, "%s%s", prefix, newPrefix);
        print_tree_node(children[i], updatedPrefix, 0, rel);
    }

    if (count > 0) {
        binary_tree_node *last = children[count - 1];
        const char *rel = (last == node->left) ? "(L) " : "(R) ";
        char updatedPrefix[100];
        sprintf(updatedPrefix, "%s%s", prefix, newPrefix);
        print_tree_node(last, updatedPrefix, 1, rel);
    }
}

int main() {
    binary_tree *tree = binary_tree_create();
    binary_tree_insert(tree, 5);
    binary_tree_insert(tree, 2);
    binary_tree_insert(tree, 3);
    binary_tree_insert(tree, 7);
    binary_tree_insert(tree, 8);
    binary_tree_insert(tree, 6);
    binary_tree_insert(tree, 1);
    print_tree_node(tree->root, "", 1, "(root) ");
    binary_tree_insert(tree, 4);
    binary_tree_delete(tree, 5);
    print_tree_node(tree->root, "", 1, "(root) ");
    print_tree_node(binary_tree_search(tree, 7), "", 1, "(7) ");
}
